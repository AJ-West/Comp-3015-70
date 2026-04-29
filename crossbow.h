#pragma once
#include "particles.h"

class crossBow {
private:
	vec3 pos;
	float yRotation = 90.0f;
	float dirRotation = 0.0f;
	bool dirInc = true;

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
		yRotation = rand() % 91;
		dirRotation = rand() % 40 - 20;
		dirInc = rand() % 2;
	}

    void update(float t, GLSLProgram* prog) {
        deltaT = t - time;
        time = t;
        updateRotation(prog);
    }

	void updateRotation(GLSLProgram* prog) { // updates roation for firing then firest when horizontal
		yRotation -= 1.0f;
		if (yRotation <= 0.0f) {
			yRotation = 90;
		}
		else if (yRotation == 5.0f) { // delay to fire the arrow first to make it look more realistic
			spawnArrow(prog);
		}
		else if (yRotation == 2.0f) { // delay to spawn smoke to time it better
			spawnParticles(prog);
		}
		if(dirInc) {
			dirRotation += 0.5f;
			if (dirRotation >= 20.0f) {
				dirInc = false;
			}
		}
		else {
			dirRotation -= 0.5f;
			if (dirRotation <= -20.0f) {
				dirInc = true;
			}
		}
	}

	void spawnArrow(GLSLProgram* prog) { // sets next availabe arrow to active at the right position		        
		for (int i = 0; i < maxArrows; i++) {
			if (!allArrows[i].inUse) {
				allArrows[i].init(pos, dir, dirRotation);
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
	float getYRotation() { return yRotation; }
	float getDirRotation() { return dirRotation; }
	int getDir() { return dir; }
};