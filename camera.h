#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext/vector_float3.hpp"

using namespace glm;

enum directions { up, left, down, right };

class Camera {
public:
	Camera(){}
	~Camera(){}

	void updatePosition(int direction) {
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
	}

	vec3 getPosition() { return position; }
	vec3 getFront() { return front; }
	vec3 getCameraUp() { return cameraUp; }

	void setCameraFront(vec3 newFront) { front = newFront; }

private:
	float speed = 0.0001f;

	//Relative position within world space
	vec3 position = vec3(0.0f, 0.0f, 0.5f);
	//The direction of travel
	vec3 front = vec3(0.0f, 0.0f, -1.0f);
	//Up position within world space
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
};