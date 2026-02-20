#pragma once

#include "camera.h"

#ifndef GLOBAL_H
#define GLOBAL_H

class Camera;

extern Camera* camera;

//Camera sideways rotation
extern float cameraYaw;
//Camera vertical rotation
extern float cameraPitch;
//if first time mouse is on window
extern bool mouseFirstEntry;
//Positions of camera from given last frame
extern float cameraLastXPos;
extern float cameraLastYPos;

#endif // !GLOBAL_H