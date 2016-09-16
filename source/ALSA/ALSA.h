// This is the Adaptive Levy Walk Controller.
// This class estimates the target distribution and generates a Levy search pattern with the appropriate exponent

#ifndef ALSA_h
#define ALSA_h

#include <utility>
#include <tuple>
#include <vector>
#include "TargetDetectionEvent.h"

using namespace std;

struct GoalState
{
  float x;
  float y;
  float yaw;
};

class ALSA
{
 public:
  ALSA();
  GoalState getNextGoalPosition(pair<float, float> current_position);
  void addDetectedTarget(time_t t, float x, float y, int id);
  void updateStrategy(); // Update the levy exponent given the state of the target vector
  float getMu();
  float getHopkinsIndex();
  ~ALSA();

 private:
  float drawFromPowerLawDistribution(float min, float max, float mu);
  float calcHopkinsIndex(vector<Coordinate*> S, float max_x, float min_x, float max_y, float min_y, float max_z, float min_z);
  float mapHopkinsToMu(float hopkins_index);
  float min_step_length; // Define the minimum step length to be 5 cm
  float max_step_length; // Define the maximum step length to be 100 m
  float mu; // mu is the PDF inverse power law exponent
  float hopkins_index;
  vector<TargetDetectionEvent> targets;
};

#endif // End ALSA_h
