#ifndef ALSA_QT_USER_FUNCTIONS_H
#define ALSA_QT_USER_FUNCTIONS_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <source/ALSA/ALSA_loop_functions.h>

using namespace argos;
using namespace std;

class ALSA_controller;
class ALSA_loop_functions;

class ALSA_qt_user_functions : public CQTOpenGLUserFunctions {

    public:

        /* constructor and destructor functions */
        ALSA_qt_user_functions();

        /* interface functions between QT and ARGoS */
        void DrawOnRobot(CFootBotEntity& entity);
        void DrawOnArena(CFloorEntity& entity);

    private:

        /* private helper drawing functions */
        void DrawNest();
        void DrawFood();
        void DrawTargetRays();

        ALSA_loop_functions& loopFunctions;

};

#endif /* ALSA_QT_USER_FUNCTIONS_H */
