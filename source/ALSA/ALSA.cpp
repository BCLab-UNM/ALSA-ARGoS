#include "ALSA.h"

#include <math.h> // for sin(), cos(), and pow()
#include <stdlib.h>  // For rand()
#include <limits> // For max and min values
#include "Coordinate.h"

using namespace std;


ALSA::ALSA()
{
  min_step_length = 0.05;
  max_step_length = 100;
  mu = -2.0;
  hopkins_index = 1;
  previous_state.x = 0;
  previous_state.y = 0;
}

GoalState ALSA::getNextGoalPosition(pair<float, float> current_position)
{
  GoalState goal_state;
  
  // We don't care about the current yaw (heading) since the movement is uncorrelated
  // Use a levy walk formulation - so uniform heading.
  float goal_yaw = (rand()*1.0/RAND_MAX)*2*M_PI;
  
  //select new position with distance selected from a power law distribution
  float step_length = drawFromPowerLawDistribution(min_step_length, max_step_length, mu);
  
  // Populate the position to return
  goal_state.x = current_position.first + (step_length * cos(goal_yaw));
  goal_state.y = current_position.second + (step_length * sin(goal_yaw));
  goal_state.yaw = goal_yaw;      

  return goal_state;
}

//Weisstein, Eric W. "Random Number." From MathWorld--A Wolfram Web Resource. http://mathworld.wolfram.com/RandomNumber.html
float ALSA::drawFromPowerLawDistribution( float min, float max, float mu )
{
  float unif_var = rand()*1.0/RAND_MAX;
  float min_power = pow(min, mu+1);
  float max_power = pow(max, mu+1);
  float base_variate =  min_power+max_power-min_power*unif_var;
  float exponent = 1/(mu+1);
  return pow(base_variate, exponent);
}

void ALSA::addDetectedTarget(time_t t, float x, float y)
{
  targets.push_back(TargetDetectionEvent(t, x, y));
}

// Update Strategy analyses the targets detected so far and chooses a new value for mu
void ALSA::updateStrategy()
{
  
  // Set mu given H

  calcHopkinsIndex();
  this->mu = mapHopkinsToMu(hopkins_index);
  
}

float ALSA::mapHopkinsToMu(float hopkins_index)
{
  // Simple lookup of the "right" mu given the hopkins index (From Robotica)
  if (hopkins_index > 0.95) return 1.7;

  return 1.4;
}

float ALSA::calcHopkinsIndex()
{
  // 1) Extract coordinates from targets vector
  // 2) Calculate the Hopkins index, H, of the coordinates
  
  // 1) Extract coordinates from targets vector and store in target_positions
  vector<Coordinate*> target_positions;
  for (std::vector<TargetDetectionEvent>::iterator TDEit = targets.begin() ; TDEit != targets.end(); ++TDEit)
    {
      target_positions.push_back(TDEit->getPositionPtr());
    }

  // Find the bounds of the targets seen so far.
  float max_x = std::numeric_limits<double>::min();
  float min_x = std::numeric_limits<double>::max();
  float max_y = std::numeric_limits<double>::min();
  float min_y = std::numeric_limits<double>::max();
  float max_z = std::numeric_limits<double>::min();
  float min_z = std::numeric_limits<double>::max();
  
  for (std::vector<Coordinate*>::iterator it = target_positions.begin() ; it != target_positions.end(); ++it)
    {
      if ((**it).getX() > max_x) max_x = (**it).getX();
      if ((**it).getX() < min_x) min_x = (**it).getX();
      if ((**it).getY() > max_y) max_y = (**it).getY();
      if ((**it).getY() < min_y) min_y = (**it).getY();
      if ((**it).getZ() > max_z) max_z = (**it).getZ();
      if ((**it).getZ() < min_z) min_z = (**it).getZ();
    }
  
  this->hopkins_index = calcHopkinsIndex(target_positions, max_x, min_x, max_y, min_y, min_z, max_z);

  return hopkins_index;
}

// Calculate the Hopkins index of the target distribution encoutered so far
float ALSA::calcHopkinsIndex(vector<Coordinate*> S, float max_x, float min_x, float max_y, float min_y, float max_z, float min_z)
{

  // The ideas is to compare the sum of the nearest neighbor distances for the locations of interest with the nearest neighbor distances of a uniformly distributed set.

    // 1) Generate a vector of uniformly distributed points in the search space, R.
    // 2) For each coordinate in S find the nearest neighbor in R.
    // 3) Keep a running sum of these nearest neighbor distances, W.
    // 4) For each coordinate in S find the nearest neighbor in locations.
    // 5) keep a running sum of these nearest neighbor distances, U.
    // 6) The normalized Hopkins statistic is U/(U+W)
    // 7) Values close to 1/2 are not clustered, values close to 1 are more clustered

    vector<Coordinate*> R;
    
    double U = 0;
    double W = 0; // Running sum for uniformly distributed nearst neighbor distances

    for(unsigned int i = 0; i < S.size(); i++)
    {
        // Generate random coordinates
        double x = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        double y = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        double z = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

        // Scale x to be between the limits of the field
        x = x*(max_x-min_x)+min_x;
        y = y*(max_y-min_y)+min_y;
        z = z*(max_z-min_z)+min_z;

        double time = 0;

        Coordinate* c = new Coordinate(x, y, z, time);
        R.push_back(c);
    }

    // Compare nearest neighbor distance sums for the uniform and observed distributions
    for (std::vector<Coordinate*>::iterator Sit = S.begin() ; Sit != S.end(); ++Sit)
    {
        double min_R_distance = std::numeric_limits<double>::max();
        double min_S_distance = std::numeric_limits<double>::max();

        for (std::vector<Coordinate*>::iterator Rit = R.begin() ; Rit != R.end(); ++Rit)
        {
          double dist = (*Sit)->getDistance(*Rit);
            if (dist < min_R_distance)
            {
                min_R_distance = dist;
            }
        }

        for (std::vector<Coordinate*>::iterator Sit2 = S.begin() ; Sit2 != S.end(); ++Sit2)
        {
          double dist = (*Sit)->getDistance(*Sit2);
            if ((*Sit) != (*Sit2) && dist < min_S_distance)
            {
                min_S_distance = dist;
            }
        }

        // Add distance to the nearest neighbor to the sum
        W += min_R_distance;
        U += min_S_distance;
    }

    double result = U/(U+W);

    //cout << "Hopkins Clustering Statistic: U//(U+W) = " << U << "//" << "(" << U << "+" << W << ") = " << result << endl;

    return result;

}

float ALSA::getHopkinsIndex()
{
  return hopkins_index;
}

float ALSA::getMu()
{
  return mu;
}

ALSA::~ALSA()
{
}
