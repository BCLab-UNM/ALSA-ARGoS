#ifndef ALSA_CONTROLLER_H
#define ALSA_CONTROLLER_H

#include <source/Base/BaseController.h>
#include <source/ALSA/ALSA_loop_functions.h>
#include "ALSA.h"

using namespace argos;
using namespace std;

class ALSA_loop_functions;

static unsigned int num_targets_collected = 0;

class ALSA_controller : public BaseController {

    public:

        ALSA_controller();

        // CCI_Controller inheritence functions
        void Init(TConfigurationNode& node);
        void ControlStep();
        void Reset();

        bool   IsHoldingFood();
	
	void SetLoopFunctions(ALSA_loop_functions* lf) { loopFunctions = lf; }
	
	argos::Real SimTimeInSeconds();

    private:

        size_t NumberOfRobots;
        size_t NumberOfSpirals;

        /* Robot ALSA state variable */
        enum STATE { SEARCHING = 1, RETURN_TO_NEST = 2, RETURN_TO_SEARCH = 3 } STATE;

        /* robot internal variables & statistics */
        CRandom::CRNG*      RNG;
        ALSA_loop_functions* loopFunctions;

        CVector2            ReturnPosition;
        
        vector<CRay3>       myTrail;
        CColor              TrailColor;

	Real                FoodDistanceTolerance;
       	CVector2            previous_position;
	CVector2            previous_target;
	CVector2            newTarget;
        CVector3            startPosition;
	bool                isHoldingFood;
        bool                goingHome;
        bool                ResetReturnPosition;
        CRange<CRadians>    AngleToleranceInRadians;
        CRange<CRadians>    Tolerance;
        size_t              stopTimeStep;
        size_t              collisionDelay;
	
        /* movement functions */
        CDegrees angleInDegrees;

	/* movement helper functions */
	bool TargetHit();
        void SetHoldingFood(); 

	string results_path;
	string results_full_path;

	// The ALSA search pattern generator
	ALSA alsa;

	// The Hopkins index calculated for the whole arena
	// This is used as a point of comparison for tracking the online partial
	// Hopkins index calculated by robots as they go.
	float global_hopkins_index;
	bool need_global_hopkins; // Flag so we only computer the global hopkins once
};

#endif /* ALSA_CONTROLLER_H */
