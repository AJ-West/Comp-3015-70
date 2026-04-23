#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext/vector_float3.hpp"

using namespace glm;

enum directions { up, left, down, right, paused };

enum {X, Y, Z};

class Camera {
public:
	Camera(){}
	~Camera(){}

	void updatePosition(int direction) { // moves the camera based off user input
		switch (direction)
		{
		case up:
			position += speed * front;
			break;
		case left:
			position -= normalize(cross(front, cameraUp)) * speed;
			break;
		case down:
			position -= speed * front;;
			break;
		case right:
			position += normalize(cross(front, cameraUp)) * speed;
			break;
		}
		if (!paused) {
			clamp(X, 0.0f, 4.0f);
			clamp(Y, 0.5f, 0.5f);
			clamp(Z, 0.0f, 5.0f);
		}
	}

	//Ensures player stays within the play area with camera at correct heigh when not paused
	void clamp(int dir, int low, int high) {
		switch (dir) {
		case X:
			if (position.x < low) {
				position.x = low;
			}
			else if (position.x > high) {
				position.x = high;
			}
			break;
		case Y:
			if (position.y < low) {
				position.y = low;
			}
			else if (position.y > high) {
				position.y = high;
			}
			break;
		case Z:
			if (position.z < low) {
				position.z = low;
			}
			else if (position.z > high) {
				position.z = high;
			}
			break;
		}
	}

	vec3 getPosition() { return position; }
	vec3 getFront() { return front; }
	vec3 getCameraUp() { return cameraUp; }
	bool getPaused() { return paused; }

	void setCameraFront(vec3 newFront) { front = newFront; }

	void togglePaused() { paused = !paused; }

private:
	float speed = 0.00025f;

	bool paused = false;

	//Relative position within world space
	vec3 position = vec3(2.5f, 0.0f, 2.5f);
	//The direction of travel
	vec3 front = vec3(0.0f, 0.0f, -1.0f);
	//Up position within world space
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
};