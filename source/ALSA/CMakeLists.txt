##
## Get the Git hash for this code
##
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")


# Find the boost filesystem package.
find_package( Boost REQUIRED COMPONENTS system filesystem )
include_directories( ${Boost_INCLUDE_DIRS} )



###############################################
# define shared object files
###############################################

add_library(ALSA_loop_functions MODULE ALSA_loop_functions.h
                                      ALSA_loop_functions.cpp
                                      ALSA_qt_user_functions.h
                                      ALSA_qt_user_functions.cpp)

add_library(ALSA_controller     SHARED ALSA_controller.h
                                      ALSA_controller.cpp
				      ALSA_loop_functions.h
                                      ALSA_loop_functions.cpp
				      ALSA.h
				      ALSA.cpp
				      TargetDetectionEvent.h
				      TargetDetectionEvent.cpp
				      Coordinate.h
				      Coordinate.cpp
				      )

###############################################
# link shared object files to dependencies
###############################################

target_link_libraries(	ALSA_controller
			BaseController
			${Boost_LIBRARIES}
			)

target_link_libraries(ALSA_loop_functions
                      ALSA_controller
                      argos3core_simulator
                      argos3plugin_simulator_entities
                      argos3plugin_simulator_footbot
                      argos3plugin_simulator_genericrobot
                      argos3plugin_simulator_qtopengl
                      ${QT_LIBRARIES}
		      ${GLUT_LIBRARY}
		      ${OPENGL_LIBRARY}
		      )
