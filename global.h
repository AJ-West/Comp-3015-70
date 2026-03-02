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


//Used for object pooling arrows
extern const int maxArrows;

struct Arrows {
	vec3 pos = vec3(0.0f, 0.0f, 0.0f);
	float rotation = 90.0f;

	int direction = 0;

	bool inUse = false;

	float speed = 0.05f;

	int lifeTime = 0;

	bool getInUse() { return inUse; }

	void init(vec3 position, int dir) { // sets arrow to right position and as active when required
		pos = position;
		direction = dir;
		inUse = true;
		lifeTime = 60;
	}

	void update() { // move arrows
		switch (direction)
		{
		case(0):
			pos.x += speed;
			break;
		case(1):
			pos.z -= speed;
			break;
		case(2):
			pos.x -= speed;
			break;
		case(3):
			pos.z += speed;
			break;
		}
		lifeTime -= 1;
		if (lifeTime == 0) { // if reached the end of its lifetime then despawn arrow by setting it to inactive
			inUse = false;
		}
	}
};

extern Arrows allArrows[];

#endif // !GLOBAL_H