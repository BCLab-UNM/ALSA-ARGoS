#include "ALSA_controller.h"
#include <boost/filesystem.hpp> // For creating missing results directory

/*****
 * Initialize most basic variables and objects here. Most of the setup should
 * be done in the Init(...) function instead of here where possible.
 *****/
ALSA_controller::ALSA_controller() :
    NumberOfRobots(0),
    NumberOfSpirals(0),
    STATE(SEARCHING),
    RNG(NULL),
    ResetReturnPosition(false),
    stopTimeStep(0),
    isHoldingFood(false)
{
  need_global_hopkins = true;
}

/*****
 * Initialize the controller via the XML configuration file. ARGoS typically
 * wants objects & variables initialized here instead of in the constructor(s).
 *****/
void ALSA_controller::Init(TConfigurationNode& node) {

    compassSensor   = GetSensor<argos::CCI_PositioningSensor>("positioning");
    wheelActuator   = GetActuator<argos::CCI_DifferentialSteeringActuator>("differential_steering");
    proximitySensor = GetSensor<argos::CCI_FootBotProximitySensor>("footbot_proximity");

    argos::TConfigurationNode settings = argos::GetNode(node, "settings");
    argos::GetNodeAttribute(settings, "NumberOfRobots",          NumberOfRobots);
    argos::GetNodeAttribute(settings, "NumberOfSpirals",         NumberOfSpirals);
    argos::GetNodeAttribute(settings, "SearchStepSize",          SearchStepSize);
    argos::GetNodeAttribute(settings, "NestDistanceTolerance", NestDistanceTolerance);
    argos::GetNodeAttribute(settings, "NestAngleTolerance", NestAngleTolerance);
    argos::GetNodeAttribute(settings, "NestAngleTolerance", NestAngleTolerance);
    argos::GetNodeAttribute(settings, "TargetDistanceTolerance", TargetDistanceTolerance);
    argos::GetNodeAttribute(settings, "TargetAngleTolerance",    TargetAngleTolerance);
    argos::GetNodeAttribute(settings, "FoodDistanceTolerance",   FoodDistanceTolerance);
    argos::GetNodeAttribute(settings, "RobotForwardSpeed",       RobotForwardSpeed);
    argos::GetNodeAttribute(settings, "RobotRotationSpeed",      RobotRotationSpeed);
    argos::GetNodeAttribute(settings, "ResultsDirectoryPath",      results_path);
    argos::GetNodeAttribute(settings, "DestinationNoiseStdev",      DestinationNoiseStdev);
    argos::GetNodeAttribute(settings, "PositionNoiseStdev",      PositionNoiseStdev);
    FoodDistanceTolerance *= FoodDistanceTolerance;

    argos::CVector2 p(GetPosition());
    SetStartPosition(argos::CVector3(p.GetX(), p.GetY(), 0.0));
    
    //startPosition = CVector3(0.0, 0.0, 0.0);
    startPosition    = CVector3(p.GetX(), p.GetY(), 0.0);

    RNG = CRandom::CreateRNG("argos");
    TrailColor = CColor(std::rand()%255, std::rand()%255, std::rand()%255, 255);

    // Name the results file with the current time and date
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    stringstream ss;

    ss << "ALSA-"<<GIT_BRANCH<<"-"<<GIT_COMMIT_HASH<<"-" 
       << (now->tm_year) << '-'
       << (now->tm_mon + 1) << '-'
       <<  now->tm_mday << '-'
       <<  now->tm_hour << '-'
       <<  now->tm_min << '-'
       <<  now->tm_sec << ".csv";

    string results_file_name = ss.str();
   results_full_path = results_path+"/"+results_file_name;        

   // Only one (the first) robot should do this:
   if (GetId().compare("ALSA_0") == 0)
     {
       
       // If the results directory specified by the user does not exist create it
       boost::filesystem::path dir(results_path);
       if(!(boost::filesystem::exists(results_path))) {
	 boost::filesystem::create_directory(results_path);
       }
       
       ofstream results_output_stream;
       results_output_stream.open(results_full_path, ios::app);
       results_output_stream << "NumberOfRobots, "
			 << "NumberOfSpirals, "
			 << "TargetDistanceTolerance, "
			 << "TargetAngleTolerance, "
			 << "SearcherGap, "
			 << "FoodDistanceTolerance, "
			 << "RobotForwardSpeed, "
			 << "RobotRotationSpeed, "
			 << "RandomSeed" << endl
			 << NumberOfRobots << ", "
			 << NumberOfSpirals << ", "
			 << TargetDistanceTolerance << ", "
			 << TargetAngleTolerance << ", "
			 << FoodDistanceTolerance << ", "
			 << RobotForwardSpeed << ", "
			 << RobotRotationSpeed << ", "
			 << CSimulator::GetInstance().GetRandomSeed() << endl;  
   results_output_stream.close();
  }

 // Use the ALSA controller to select the initial goal location
 pair<float, float> current_position = make_pair(previous_position.GetX(), previous_position.GetY()); 
 
 GoalState goal_state = alsa.getNextGoalPosition( current_position );
 SetTarget(CVector2(goal_state.x, goal_state.y));


 cout << "Finished Initializing the ALSA" << endl;
}

/*****
 * Primary control loop for this controller object. This function will execute
 * the ALSA logic once per frame.
 *****/
void ALSA_controller::ControlStep() 
{
  
  // Expensive to do here... TODO: mark with a flag so only happens once
  // cant do in init because loopFunctions may not have compeleted its init by then
  // Calculate the Hopkins index for this distribution of targets
  // 1) Extract coordinates from targets (food) list and store in target_positions
 
  if (need_global_hopkins)
    {
      vector<Coordinate*> target_positions;
      vector<CVector2> food_list = loopFunctions->FoodList;
      for (std::vector<CVector2>::iterator food_itr = food_list.begin() ; food_itr != food_list.end(); ++food_itr)
	{
	  target_positions.push_back(new Coordinate(food_itr->GetX(), food_itr->GetY(), 0, 0));
	}
      
      global_hopkins_index = alsa.calcHopkinsIndex( target_positions );
      need_global_hopkins = false;
    }
   

  // To draw paths
  if (STATE == SEARCHING)
    {
      CVector3 position3d(GetPosition().GetX(), GetPosition().GetY(), 0.00);
      CVector3 target3d(previous_position.GetX(), previous_position.GetY(), 0.00);
      CRay3 targetRay(target3d, position3d);
      myTrail.push_back(targetRay);
      
      loopFunctions->TargetRayList.push_back(targetRay);
      loopFunctions->TargetRayColorList.push_back(TrailColor);
    }
  
  //LOG << myTrail.size() << endl;
  previous_position = GetPosition();
  
  /* Continue in a sprial */
  if( STATE == SEARCHING )
    {
      SetIsHeadingToNest(false);
      
      // SetHoldingFood checks if food can be picked up
      SetHoldingFood();
      if (IsHoldingFood())
	{
	  Stop(); // Stops the current movement
	  STATE = RETURN_TO_NEST;
	  SetIsHeadingToNest(true);
	  SetTarget(loopFunctions->NestPosition);
	  ReturnPosition = GetPosition();

	  // Record the position of the food detected.
	  alsa.addDetectedTarget(SimulationTick(), previous_position.GetX(), previous_position.GetY());
	  
	  // update the ALSA strategy
	  alsa.updateStrategy();
	  
	  // For the first robot 
	  if (GetId().compare("ALSA_0") == 0)
	    {
	      float hopkins_index = alsa.getHopkinsIndex();
	      float mu = alsa.getMu();
	      
	      argos::LOG << loopFunctions->Score() << ", " << hopkins_index << "("<< global_hopkins_index << ": " << 100*fabs(hopkins_index-global_hopkins_index)/global_hopkins_index <<"%)" << ", " << mu << std::endl;

	      ofstream results_output_stream;
	      results_output_stream.open(results_full_path, ios::app);
	      
	      results_output_stream << loopFunctions->Score() << ", " << hopkins_index << ", " << mu << "\n";
	      results_output_stream.close();
	    }
	}
      else // Continue searching
 	{
	  
	  if ( IsAtTarget() ) // Are we there yet? If so pick a new destination target
	    {
	      // Use the ALSA controller to select the next goal location
	      
	      pair<float, float> current_position = make_pair(previous_position.GetX(), previous_position.GetY()); 
	      
	      GoalState goal_state = alsa.getNextGoalPosition( current_position );
	      SetTarget(CVector2(goal_state.x, goal_state.y));

	      float mu = alsa.getMu();
	      float hopkins_index = alsa.getHopkinsIndex();
	      float length = sqrt((current_position.first - goal_state.x)*(current_position.first - goal_state.x)+(current_position.second - goal_state.y)*(current_position.second - goal_state.y));
	     
	    }
	  //	  argos::LOG << "SEARCHING" << std::endl;
	}
    }
  else if( STATE == RETURN_TO_NEST) 
    {
      //argos::LOG << "RETURN_TO_NEST" << std::endl;
      SetIsHeadingToNest(true);
      // Check if we reached the nest. If so record that we dropped food off and go back to the spiral
      if((GetPosition() - loopFunctions->NestPosition).SquareLength() < loopFunctions->NestRadiusSquared) 
	{
	  STATE = RETURN_TO_SEARCH;
	  SetIsHeadingToNest(false);
	  
	  if (isHoldingFood)
	    {
	      num_targets_collected++;
	      loopFunctions->setScore(num_targets_collected);
	    }
	  
	  isHoldingFood = false;
	  
	  /*
	    ofstream results_output_stream;
	    results_output_stream.open(results_full_path, ios::app);
	    results_output_stream << loopFunctions->getSimTimeInSeconds() << ", " << ++num_targets_collected << ", " << "Col Count" << endl;	    
	    results_output_stream.close();
	  */
	  
	}
       else
	{
	  SetIsHeadingToNest(true);
	  SetTarget(loopFunctions->NestPosition);
	}
    }
  else if( STATE == RETURN_TO_SEARCH ) 
    {
      // argos::LOG << "RETURN_TO_SEARCH" << std::endl;
      SetIsHeadingToNest(false);

      //argos::LOG << "Return Position:" << ReturnPosition << endl;
      //argos::LOG << "Robot position:" << GetPosition() << endl;
      //argos::LOG << "Target position:" << GetTarget() << endl;
      //argos::LOG << "Distance:" << (GetPosition() - ReturnPosition).Length() << endl;
      //argos::LOG << "Distance Tolerance:" << TargetDistanceTolerance << endl;
      
      // Check if we have reached the return position
      if ( IsAtTarget() )
	{
	  //argos::LOG << "RETURN_TO_SEARCH: Pattern Point" << std::endl;
	  SetIsHeadingToNest(false);
	  //SetTarget(ReturnPosition);
	  STATE = SEARCHING;
	}
      else
	{
	  SetTarget(ReturnPosition);
	}
    } 
  
  
  Move();
}

/*****
 * Returns a boolean based on weather the robot is with 0.01 
 * distance tolerance. Declares that the robot had reached 
 * current target.
 *****/
 bool ALSA_controller::TargetHit() {
    CVector2 position = GetPosition() - GetTarget();
    bool hit = false;
     
    if(position.SquareLength() < TargetDistanceTolerance){
        hit = true;
    }
    return hit;
 }

/*****
 * Check if the Robot is finding food. This is defined as the Robot being within
 * the distance tolerance of the position of a food item. If the Robot has found
 * food then the appropriate boolean flags are triggered.
 *****/
void ALSA_controller::SetHoldingFood(){
    if(IsHoldingFood() == false) 
      {
        vector <CVector2> newFoodList; 
        size_t i = 0;
        for (i = 0; i < loopFunctions->FoodList.size(); i++)
	  {
            if ((GetPosition()-loopFunctions->FoodList[i]).SquareLength() < FoodDistanceTolerance && !isHoldingFood)
	      {
		isHoldingFood = true;
	      } 
	    else 
	      {
		newFoodList.push_back(loopFunctions->FoodList[i]);
	      }
        } 
        loopFunctions->FoodList = newFoodList;
      }

}

/*****
 * Is this Robot_controller holding food?
 *     true  = yes
 *     false = no
 *****/
bool ALSA_controller::IsHoldingFood() {
    return isHoldingFood;
}
/*****
 * After pressing the reset button in the GUI, this controller will be set to
 * default factory settings like at the start of a simulation.
 *****/
void ALSA_controller::Reset() {
    collisionDelay  = 0;
    SetIsHeadingToNest(true);
    SetTarget(loopFunctions->NestPosition);
}

REGISTER_CONTROLLER(ALSA_controller, "ALSA_controller")
