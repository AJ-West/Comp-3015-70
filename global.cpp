#include "global.h"

#include "camera.h"

Camera* camera = new Camera();

//Camera sideways rotation
float cameraYaw = -90.0f;
//Camera vertical rotation
float cameraPitch = 0.0f;
//if first time mouse is on window
bool mouseFirstEntry = true;
//Positions of camera from given last frame
float cameraLastXPos = 800.0f / 2.0f;
float cameraLastYPos = 600.0f / 2.0f;

const int maxArrows = 100;

Arrows allArrows[maxArrows];