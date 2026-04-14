#pragma once
#include "global.h"
#include "media/texture/texture.h"
#include "helper/particleutils.h"

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

public:
	crossBow(vec3 position, int direction) :pos(position), dir(direction) {
		rotation = rand() % 91;
	}

    void update(float t) {
        deltaT = t - time;
        time = t;
        updateRotation();
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
		spawnParticles();
		for (int i = 0; i < maxArrows; i++) {
			if (!allArrows[i].inUse) {
				allArrows[i].init(pos, dir);
				break;
			}
		}
	}

	void spawnParticles() {
       // initBuffers();
	}

	void initBuffers(GLSLProgram* prog) {
        // Generate the buffer for initial velocity and start (birth) time
        glGenBuffers(2, posBuf); // position buffers
        glGenBuffers(2, velBuf); // velocity buffers
        glGenBuffers(2, age); // age buffers

        //Allocate space for all buffers
        int size = nParticles * 3 * sizeof(GLfloat);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
        glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
        glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
        glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
        glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, age[0]);
        glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), 0, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, age[1]);
        glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), 0, GL_DYNAMIC_COPY);

        // Fill the first age buffer
        std::vector<GLfloat> tempData(nParticles);
        float rate = particleLifetime / nParticles;
        for (int i = 0; i < nParticles; i++) {
            tempData[i] = rate * (i - nParticles);
        }
        glBindBuffer(GL_ARRAY_BUFFER, age[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), tempData.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // create vertex arrays for each set of buffers
        glGenVertexArrays(2, particleArray);

        // set up particle array 0
        glBindVertexArray(particleArray[0]);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, age[0]);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        // set up particle array 1
        glBindVertexArray(particleArray[1]);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, age[1]);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        // Set up the feedback objects
        glGenTransformFeedbacks(2, feedback);

        // Transform feedback 0
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

        // Transform feedback 1
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        setProgSettings(prog);

        glActiveTexture(GL_TEXTURE1);
        ParticleUtils::createRandomTex1D(nParticles * 3);
	}

    void setProgSettings(GLSLProgram* prog) {
        prog->use();
        prog->setUniform("RandomTex", 1);
        prog->setUniform("ParticleTex", 0);
        prog->setUniform("ParticleLifetime", particleLifetime);
        prog->setUniform("Accel", vec3(0.0f, 0.0f, 0.0f));
        prog->setUniform("ParticleSize", 0.5f);
        //prog->setUniform("Emitter", emitterPos + pos);
        prog->setUniform("Emitter", emitterPos);
        prog->setUniform("EmitterBasis", ParticleUtils::makeArbitraryBasis(emitterDir));
    }

    void updateParticles(GLSLProgram* prog) {
        setProgSettings(prog);

        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
        glBeginTransformFeedback(GL_POINTS);

        glBindVertexArray(particleArray[1 - drawBuf]);
        glVertexAttribDivisor(0, 0);
        glVertexAttribDivisor(1, 0);
        glVertexAttribDivisor(2, 0);
        glDrawArrays(GL_POINTS, 0, nParticles);
        glBindVertexArray(0);

        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
    }

    void renderParticles(GLSLProgram* prog) {
        setProgSettings(prog);

        glDepthMask(GL_FALSE);
        glBindVertexArray(particleArray[drawBuf]);
        glVertexAttribDivisor(0, 1);
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);

        //swap buffers
        drawBuf = 1 - drawBuf;
    }

	vec3 getPos() { return pos; }
	float getRotation() { return rotation; }
	int getDir() { return dir; }
};