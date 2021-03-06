#include "ALSA_qt_user_functions.h"

/*****
 * Constructor: In order for drawing functions in this class to be used by
 * ARGoS it must be registered using the RegisterUserFunction function.
 *****/
ALSA_qt_user_functions::ALSA_qt_user_functions() :
    loopFunctions(dynamic_cast<ALSA_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions()))
{
    RegisterUserFunction<ALSA_qt_user_functions, CFootBotEntity>(&ALSA_qt_user_functions::DrawOnRobot);
    RegisterUserFunction<ALSA_qt_user_functions, CFloorEntity>(&ALSA_qt_user_functions::DrawOnArena);
}

/*****
 *
 *****/
void ALSA_qt_user_functions::DrawOnRobot(CFootBotEntity& entity) {
    //ALSA_controller& c = dynamic_cast<ALSA_controller&>(entity.GetControllableEntity().GetController());
    ALSA_controller& c = dynamic_cast<ALSA_controller&>(entity.GetControllableEntity().GetController());

    if(c.IsHoldingFood() == true) {
        DrawCylinder(CVector3(0.0, 0.0, 0.3), CQuaternion(), loopFunctions.FoodRadius, 0.025, CColor::BLACK);
    }

    if(loopFunctions.DrawIDs == 1) {
        /* Disable lighting, so it does not interfere with the chosen text color */
        glDisable(GL_LIGHTING);
        /* Disable face culling to be sure the text is visible from anywhere */
        glDisable(GL_CULL_FACE);
        /* Set the text color */
        CColor cColor(CColor::BLACK);
        glColor3ub(cColor.GetRed(), cColor.GetGreen(), cColor.GetBlue());

        /* The position of the text is expressed wrt the reference point of the footbot
         * For a foot-bot, the reference point is the center of its base.
         * See also the description in
         * $ argos3 -q foot-bot
         */
        GetQTOpenGLWidget().renderText(0.0, 0.0, 0.5,             // position
                                    entity.GetId().c_str()); // text
        /* Restore face culling */
        glEnable(GL_CULL_FACE);
        /* Restore lighting */
        glEnable(GL_LIGHTING);
    }
}
 
/*****
 *
 *****/
void ALSA_qt_user_functions::DrawOnArena(CFloorEntity& entity) {
    DrawFood();
    DrawNest();
    DrawTargetRays();
}

/*****
 * This function is called by the DrawOnArena(...) function. If the ALSA_data
 * object is not initialized this function should not be called.
 *****/
void ALSA_qt_user_functions::DrawNest() {

    /* 2d cartesian coordinates of the nest */
    Real x_coordinate = loopFunctions.NestPosition.GetX();
    Real y_coordinate = loopFunctions.NestPosition.GetX();

    /* required: leaving this 0.0 will draw the nest inside of the floor */
    Real elevation = loopFunctions.NestElevation;

    /* 3d cartesian coordinates of the nest */
    CVector3 nest_3d(x_coordinate, y_coordinate, elevation);

    /* Draw the nest on the arena. */
    DrawCircle(nest_3d, CQuaternion(), loopFunctions.NestRadius, CColor::GRAY50);
}

/*****
 *
 *****/
void ALSA_qt_user_functions::DrawFood() {

    Real x, y;

    for(size_t i = 0; i < loopFunctions.FoodList.size(); i++) {
        x = loopFunctions.FoodList[i].GetX();
        y = loopFunctions.FoodList[i].GetY();
        DrawCylinder(CVector3(x, y, 0.0), CQuaternion(), loopFunctions.FoodRadius, 0.025, loopFunctions.FoodColoringList[i]);
    }
}

void ALSA_qt_user_functions::DrawTargetRays() {

  return;
	//size_t tick = loopFunctions.GetSpace().GetSimulationClock();
	//size_t tock = loopFunctions.GetSimulator().GetPhysicsEngine("default").GetInverseSimulationClockTick() / 8;

    //if(tock == 0) tock = 1;

    //if(tick % tock == 0) {

        for(size_t j = 0; j < loopFunctions.TargetRayList.size(); j++) {

            DrawRay(loopFunctions.TargetRayList[j], loopFunctions.TargetRayColorList[j]);
      
        }
    //}
}

REGISTER_QTOPENGL_USER_FUNCTIONS(ALSA_qt_user_functions, "ALSA_qt_user_functions")
