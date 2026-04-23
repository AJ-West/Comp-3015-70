#pragma once
#include "particles.h"

class crossBow {
private:
	vec3 pos;
	float rotation = 90.0f;

	int dir;

	int delayCount = 0;

	int nParticles = 40;
	float time = 0;
	float particleLifetime = 0.5f;
	float deltaT = 0;

	// position and direction of particle emitter
	vec3 emitterPos = vec3(1,0,0);
	vec3 emitterDir = vec3(0,2,0);

	//particle buffers
	GLuint posBuf[2];
	GLuint velBuf[2];
	GLuint age[2];

	//particle VAOs
	GLuint particleArray[2];

	// Transform feedbacks
	GLuint feedback[2];

	GLuint drawBuf = 1;

    particles* curParticles = nullptr;

public:
	crossBow(vec3 position, int direction, GLSLProgram* prog) :pos(position), dir(direction) {
		rotation = rand() % 91;
	}

    void update(float t, GLSLProgram* prog) {
        deltaT = t - time;
        time = t;
        updateRotation(prog);
    }

	void updateRotation(GLSLProgram* prog) { // updates roation for firing then firest when horizontal
		rotation -= 0.01f;
		if (rotation <= 0.0f) {            
			rotation = 90;
			spawnArrow(prog);
			spawnParticles(prog);
		}
		else if (rotation == 0.05f) { // delay to fire the arrow first to make it look more realistic
			spawnArrow(prog);
		}
		else if (rotation == 0.02f) { // delay to spawn smoke to time it better
			spawnParticles(prog);
		}
	}

	void spawnArrow(GLSLProgram* prog) { // sets next availabe arrow to active at the right position		        
		for (int i = 0; i < maxArrows; i++) {
			if (!allArrows[i].inUse) {
				allArrows[i].init(pos, dir);
				break;
			}
		}
	}

	void spawnParticles(GLSLProgram* prog) {
        delete curParticles;
        curParticles = nullptr;
        curParticles = new particles(prog, dir);
	}

    void updateParticles(GLSLProgram* prog) {
		if (curParticles) {
			curParticles->updateParticles();
		}
    }

    void renderParticles(GLSLProgram* prog) {
		if (curParticles) {
			curParticles->renderParticles();
		}
    }

	vec3 getPos() { return pos; }
	float getRotation() { return rotation; }
	int getDir() { return dir; }
};