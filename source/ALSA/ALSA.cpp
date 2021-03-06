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

  max_x = min_x = min_y = max_y = 0.0f;
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

  this->hopkins_index = calcHopkinsIndex();
  this->mu = mapHopkinsToMu(hopkins_index);
  
}

float ALSA::mapHopkinsToMu(float hopkins_index)
{
  // Simple lookup of the "right" mu given the hopkins index (From Robotica)
  if (hopkins_index > 0.95) return -1.8;

  return -1.2;
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
  /*
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
  
  */ // Use the arena bounds - otherwise we are measuring the structure within the clusters instead of globally...? 
  
  // Why 20?

  float hopkins_index = 0;
  float n_reps = 20;
  for (int i = 0; i < n_reps; i++)
    hopkins_index += calcHopkinsIndex(target_positions, max_x, min_x, max_y, min_y, 0, 0);

  return hopkins_index/n_reps;
}

// Convenience version of calcHopkins index that just takes a set of points.
// Todo: refactor so this is the only version of calcHopkinsStatistic used
float ALSA::calcHopkinsIndex(vector<Coordinate*> S)
{
  // Find the bounds of the targets seen so far.
  /*
  float max_x = std::numeric_limits<double>::min();
  float min_x = std::numeric_limits<double>::max();
  float max_y = std::numeric_limits<double>::min();
  float min_y = std::numeric_limits<double>::max();
  float max_z = std::numeric_limits<double>::min();
  float min_z = std::numeric_limits<double>::max();
  
  for (std::vector<Coordinate*>::iterator it = S.begin() ; it != S.end(); ++it)
    {
      if ((**it).getX() > max_x) max_x = (**it).getX();
      if ((**it).getX() < min_x) min_x = (**it).getX();
      if ((**it).getY() > max_y) max_y = (**it).getY();
      if ((**it).getY() < min_y) min_y = (**it).getY();
      if ((**it).getZ() > max_z) max_z = (**it).getZ();
      if ((**it).getZ() < min_z) min_z = (**it).getZ();
    }
  */
    
  // Why 20?
  float hopkins_index = 0;
  float n_reps = 20;
  for (int i = 0; i < n_reps; i++)
    hopkins_index += calcHopkinsIndex(S, max_x, min_x, max_y, min_y, 0, 0);
  
  return hopkins_index/n_reps;
}

// Calculate the Hopkins index of the target distribution encoutered so far
// n_points is the number of nearest neighbors to add to the score
float ALSA::calcHopkinsIndex(vector<Coordinate*> S, float max_x, float min_x, float max_y, float min_y, float max_z, float min_z)
{

  // Sample size for the S and R point sets
  unsigned int n_points = 20;
  
  // The ideas is to compare the sum of the nearest neighbor distances for the locations of interest with the nearest neighbor distances of a uniformly distributed set.

    // 1) Generate a vector of uniformly distributed points in the search space, R.
    // 2) For each coordinate in S find the nearest neighbor in R.
    // 3) Keep a running sum of these nearest neighbor distances, W.
    // 4) For each coordinate in S find the nearest neighbor in locations.
    // 5) keep a running sum of these nearest neighbor distances, U.
    // 6) The normalized Hopkins statistic is U/(U+W)
    // 7) Values close to 1/2 are not clustered, values close to 1 are more clustered

    vector<Coordinate*> R;

    // These will point to subsets of R and S of size n_points. The the sum of the min distances between
    // the points in the these subsets and all the points in R and S will be U and W below.
    vector<Coordinate*> S_subset;
    vector<Coordinate*> R_subset;
    
    
    double U = 0; // Running sum for uniformly distributed nearst neighbor distances
    double W = 0; // Running sim for observed inter-target distances

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

    // Populate R_subset and S_subset with n_points number of elements from R and S respectively
    for (int i = 0; i < n_points; i++)
      {
	R_subset.push_back(R[rand()%R.size()]);
	S_subset.push_back(S[rand()%S.size()]);
      }
    
    // Compare nearest neighbor distance sums for the uniform and observed distributions
    for (std::vector<Coordinate*>::iterator Sit = S.begin() ; Sit != S.end(); ++Sit)
    {
        double min_R_distance = std::numeric_limits<double>::max();
        double min_S_distance = std::numeric_limits<double>::max();

        for (std::vector<Coordinate*>::iterator R_subset_itr = R_subset.begin() ; R_subset_itr != R_subset.end(); ++R_subset_itr)
        {
          double dist = (*Sit)->getDistance(*R_subset_itr);
	  if (dist < min_R_distance)
            {
	      min_R_distance = dist;
            }
        }

        for (std::vector<Coordinate*>::iterator S_subset_itr = S_subset.begin() ; S_subset_itr != S_subset.end(); ++S_subset_itr)
        {
          double dist = (*Sit)->getDistance(*S_subset_itr);
	  if ((*Sit) != (*S_subset_itr) && dist < min_S_distance)
            {
	      min_S_distance = dist;
            }
        }

        // Add distance to the nearest neighbor to the sum
        U += min_R_distance;
        W += min_S_distance;
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

void ALSA::setMaxX(float x)
{
  max_x = x;
}

void ALSA::setMinX(float x)
{
  min_x = x;
}

void ALSA::setMaxY(float y)
{
  max_y = y;
}

void ALSA::setMinY(float y)
{
  min_y = y;
}

void ALSA::setMu(float mu)
{
  this->mu = mu;
}

ALSA::~ALSA()
{
}
