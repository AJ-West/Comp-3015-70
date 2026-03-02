#pragma once
#include "global.h"

class crossBow {
private:
	vec3 pos;
	float rotation = 90.0f;

	int dir;

	int delayCount = 0;

public:
	crossBow(vec3 position, int direction) :pos(position), dir(direction) {
		rotation = rand() % 91;
	}

	void updateRotation() { // updates roation for firing then firest when horizontal
		rotation -= 1.0f;
		if (rotation <= 0.0f) {
			rotation = 90;
		}
		else if (rotation == 5.0f) { // delay to fire the arrow first to make it look more realistic
			spawnArrow();
		}
	}

	void spawnArrow() { // sets next availabe arrow to active at the right position
		for (int i = 0; i < maxArrows; i++) {
			if (!allArrows[i].inUse) {
				allArrows[i].init(pos, dir);
				break;
			}
		}
	}

	vec3 getPos() { return pos; }
	float getRotation() { return rotation; }
	int getDir() { return dir; }
};