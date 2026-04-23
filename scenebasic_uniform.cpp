#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <ctime>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include <glm/gtc/matrix_transform.hpp>

#include "helper/noisetex.h"

#include "global.h"
#include "crossbow.h"

using glm::vec3;

using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : sky(100.0f), angle(0.0f), drawBuf(1), cTime(0), deltaT(0), nParticles(400),
particleLifetime(1.5f), emitterPos(1, 0, 0), emitterDir(0, 2, 0) {
	tPrev = 0;
	angle = 0;
}

void SceneBasic_Uniform::initScene()
{
	std::srand(time(0));

    compile();

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	//sets up models for default arrow
	model1 = mat4(1.0f);

	model1 = translate(model1, vec3(2.0f, 0.0f, 0.0f));
	model1 = scale(model1, vec3(0.025f, 0.025f, 0.025f));

	//sets up default model for crossbow
	model2 = mat4(1.0f);

	model2 = glm::rotate(model2, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	model2 = scale(model2, vec3(0.025f, 0.025f, 0.025f));	

	// set view to inital camera position
	view = lookAt(camera->getPosition(), camera->getPosition() + camera->getFront(), camera->getCameraUp());

	projection = mat4(1.0f);

	// sets up models and textures
	prog.use();

	setupFBO();

	prog.setUniform("Model", model1);

	setProgDefaults(&prog);

	//loads all textures that will be required
	metalTex = Texture::loadTexture("media/texture/metal.png");
	metalNormal = Texture::loadTexture("media/texture/metal/metal_normalMap.png");
	rustTex = Texture::loadTexture("media/texture/rust/rust.png");
	rustNormal = Texture::loadTexture("media/texture/rust/rust_normalMap.png");
	woodTex = Texture::loadTexture("media/texture/wood/wood.png");
	woodNormal = Texture::loadTexture("media/texture/wood/wood_normalMap.png");

	flagTex = Texture::loadTexture("media/texture/flag.png");

	GLuint tex1 = Texture::loadTexture("media/texture/metal.png");
	GLuint norm1 = Texture::loadTexture("media/texture/metal/metal_normalMap.png");
	GLuint tex2 = Texture::loadTexture("media/texture/rust/rust.png");
	GLuint norm2 = Texture::loadTexture("media/texture/rust/rust_normalMap.png");

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, norm1);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, tex2);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, norm2);

	// loads required models
	arrow = ObjMesh::load("media/models/arrow.obj", false, true);
	crossbow = ObjMesh::load("media/models/crossbow.obj", false, true);

	//sets up skybox
	skyModel = mat4(1.0f);

	//loads cubemap texture
	cubeTex = Texture::loadHdrCubeMap("media/texture/mountainsCubeMap/skybox");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	// Spawns the crossbows along each side in the required positions with the correct direction to face
	int side = -1;
	vec3 pos;
	for (int i = 0; i < 16; i++) {
		pos = vec3(0.0f, 0.0f, 0.0f);
		if (i % 4 == 0) { side += 1; }
		int dir = 0;
		switch (side)
		{
		case 0:
			pos = vec3(pos.x + 1.0f * i + 0.5f, pos.y, pos.z);
			dir = 3;
			break;
		case 1:
			pos = vec3(pos.x + 4.0f, pos.y, pos.z + 1.0f * (i - 3));
			dir = 2;
			break;
		case 2:
			pos = vec3(pos.x + 1.0f * (i - 8) + 0.5f, pos.y, pos.z + 5.0f);
			dir = 1;
			break;
		case 3:
			pos = vec3(pos.x, pos.y, pos.z + 1.0f * (i - 11));
			dir = 0;
			break;
		
		}
		crossbows.emplace_back(new crossBow(pos, dir, &pProg));
	}
	
	createCloudQuad();

	pProg.use();

	model = mat4(2.0f);

	smoke = Texture::loadTexture("media/texture/smoke.png");
	particleTex = ParticleUtils::createRandomTex1D(nParticles * 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, smoke);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, particleTex);
	
	pProg.setUniform("RandomTex", 1);
	pProg.setUniform("ParticleTex", 0);
	pProg.setUniform("ParticleLifetime", particleLifetime);
	pProg.setUniform("Accel", vec3(0.0f, 0.0f, 0.0f));
	pProg.setUniform("ParticleSize", 0.5f);
	pProg.setUniform("Emitter", emitterPos);
	pProg.setUniform("EmitterBasis", ParticleUtils::makeArbitraryBasis(emitterDir));

	prog.use();

	flags.emplace_back(new Flag(vec3(2.0, 1.0, 0.0), vec3(90.0, 0.0, 0.0)));
	flags.emplace_back(new Flag(vec3(4.0, 1.0, 2.0), vec3(90.0, 90.0, 0.0)));
	flags.emplace_back(new Flag(vec3(2.0, 1.0, 4.0), vec3(90.0, 180.0, 0.0)));
	flags.emplace_back(new Flag(vec3(0.0, 1.0, 2.0), vec3(90.0, 270.0, 0.0)));
}

void SceneBasic_Uniform::setProgDefaults(GLSLProgram* cProg) {
	//sets up inital location for moving lights (green and brown)
	float x, z;
	for (int i = 1; i < 3; i++) {
		std::stringstream name;
		name << "Lights[" << i << "].Position";
		x = 5.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 5.0f * sinf((glm::two_pi<float>() / 3) * i);
		cProg->setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
	}
	//position and colour of stationary main light
	cProg->setUniform("Lights[0].Position", vec4(2.5f, 1.0f, 2.5f, 1.0f));
	cProg->setUniform("Lights[0].Ld", vec3(1.0f, 1.0f, 1.0f));
	cProg->setUniform("Lights[0].La", vec3(0.2f, 0.2f, 0.2f));
	cProg->setUniform("Lights[0].Ls", vec3(1.0f, 1.0f, 1.0f));

	//colours of moving lights
	//green
	cProg->setUniform("Lights[1].Ld", vec3(0.0f, 0.4f, 0.0f));
	cProg->setUniform("Lights[1].La", vec3(0.0f, 0.1f, 0.0f));
	cProg->setUniform("Lights[1].Ls", vec3(0.0f, 0.4f, 0.0f));
	
	//brown
	cProg->setUniform("Lights[2].Ld", vec3(0.3f, 0.1f, 0.1f));
	cProg->setUniform("Lights[2].La", vec3(0.15f, 0.05f, 0.05f));
	cProg->setUniform("Lights[2].Ls", vec3(0.3f, 0.1f, 0.1f));

	//Setting up variables for the fog
	cProg->setUniform("Fog.MaxDist", 10.0f);
	cProg->setUniform("Fog.MinDist", 1.0f);
	cProg->setUniform("Fog.Colour", vec3(0.0f, 0.0f, 0.0f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();

		pProg.compileShader("shader/particles.vert");
		pProg.compileShader("shader/particles.frag");

		// setup transform feedback
		GLuint progHandle = pProg.getHandle();
		const char* outputNames[] = { "Position", "Velocity", "Age" };
		glTransformFeedbackVaryings(progHandle, 3, outputNames, GL_SEPARATE_ATTRIBS);

		pProg.link();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	// update view for current camera position
	view = lookAt(camera->getPosition(), camera->getPosition() + camera->getFront(), camera->getCameraUp());

	if (!camera->getPaused()) { // only runs if game isn't paused
		//updating light position	
		angle += 0.5f * (t - tPrev);

		tPrev = t;

		if (angle > glm::two_pi<float>()) {
			angle -= glm::two_pi<float>();
		}

		//update all crossbow's time and rotation
		pProg.use();
		for (int i = 0; i < 16; i++) {
			crossbows[i]->update(t, &pProg);
		}
		prog.use();

		// updates position for all arrows and checks if they hit the player
		for (int i = 0; i < maxArrows; i++) {
			if (allArrows[i].inUse) {
				allArrows[i].update();
				//can ignore y due to it being clamped at set height
				float xDist = allArrows[i].pos.x - camera->getPosition().x;
				float zDist = allArrows[i].pos.z - camera->getPosition().z;

				float dist = sqrt(xDist * xDist + zDist * zDist);
				if (dist < 0.035f) {
					std::cout << "shot" << std::endl;
					exit(EXIT_SUCCESS);
				}

			}
		}
	}

	deltaT = t - cTime;
	cTime = t;

	//updates clouds
	glm::vec2 offset = glm::vec2(0.0f);
	offset.x += t / 100;
	offset.y += t / 200;

	prog.setUniform("offset", offset);
}

void SceneBasic_Uniform::updateCamera(int direction) {
	if (direction == paused) { camera->togglePaused(); } // if p is pressed
	camera->updatePosition(direction);
}

void SceneBasic_Uniform::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection = glm::perspective(glm::radians(70.0f), (float)width / height, 0.01f, 100.0f);
	drawScene();
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.01f, 100.0f);
}

void SceneBasic_Uniform::setMatrices(mat4 model, GLSLProgram* cProg) {
	mat4 mv = view * model;

	cProg->setUniform("camPos", camera->getPosition());
	
	cProg->setUniform("ModelViewMatrix", mv);

	cProg->setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

	cProg->setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setupFBO() {
	GLuint depthBuf;
	//Create and bind FBO
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	//the depth buffer
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	//the HDR colour buffer
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &hdrTex);
	glBindTexture(GL_TEXTURE_2D, hdrTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Attach the images to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTex, 0);
	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(2, drawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneBasic_Uniform::drawScene() {
	prog.use();
	//draws skybox
	prog.setUniform("isSkybox", true);
	skyModel = mat4(1.0f);
	setMatrices(skyModel, &prog);
	prog.setUniform("MVP", projection * view * skyModel);
	prog.setUniform("Model", skyModel);
	sky.render();

	prog.setUniform("isSkybox", false);

	prog.setUniform("isClouds", true);
	cloudModel = mat4(1.0f);
	cloudModel = translate(cloudModel, vec3(0.0f, 15.0f, 0.0f));
	cloudModel = rotate(cloudModel, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	cloudModel = scale(cloudModel, vec3(20.0f, 20.0f, 20.0f));

	setMatrices(cloudModel, &prog);
	prog.setUniform("Model", cloudModel);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glBindVertexArray(cloudQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	prog.setUniform("isClouds", false);

	//updates position of the moving lights
	float x, z;
	for (int i = 1; i < 3; i++) {
		std::stringstream name;
		name << "Lights[" << i << "].Position";
		x = 5.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 5.0f * sinf((glm::two_pi<float>() / 3) * i);
		prog.setUniform(name.str().c_str(), view * glm::vec4(x * cos(angle), 1.2f, (z + 1.0f) * sin(angle), 1.0f));
	}

	prog.setUniform("numOfTex", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, woodTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, woodNormal);
	//render each arrow
	for (int i = 0; i < maxArrows; i++) {
		if (allArrows[i].inUse) {
			model1 = mat4(1.0f);			
			model1 = translate(model1, allArrows[i].pos);
			model1 = rotate(model1, radians(allArrows[i].rotation * allArrows[i].direction), vec3(0.0f, 1.0f, 0.0f));
			model1 = scale(model1, vec3(0.01f, 0.01f, 0.01f));
			setMatrices(model1, &prog);
			prog.setUniform("Model", model1);
			
			arrow->render();
		}
	}
	model1 = mat4(1.0f);

	prog.setUniform("numOfTex", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metalTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, metalNormal);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, rustTex);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rustNormal);
	//render each crossbow
	for (int i = 0; i < 16; i++) {
		prog.use();
		model2 = mat4(1.0f);		
		model2 = translate(model2, crossbows[i]->getPos());
		model2 = rotate(model2, radians(90.0f * (crossbows[i]->getDir()+1)), vec3(0.0f, 1.0f, 0.0f));
		model2 = glm::rotate(model2, glm::radians(crossbows[i]->getRotation()), vec3(1.0f, 0.0f, 0.0f));
		model2 = scale(model2, vec3(0.025f, 0.025f, 0.025f));
		setMatrices(model2, &prog);
		prog.setUniform("Model", model2);		
		crossbow->render();
	}
	model2 = mat4(1.0f);

	renderParticles();

	prog.setUniform("flag", true);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, flagTex);
	
	for (auto& flag : flags) {
		flag->render(&prog, model, view, projection, cTime);
	}

	prog.setUniform("flag", false);
}

void SceneBasic_Uniform::renderParticles() {
	pProg.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, smoke);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, particleTex);

	pProg.setUniform("Time", cTime);
	pProg.setUniform("DeltaT", deltaT);	

	for (int i = 0; i < 16; i++) {
		model = mat4(1.0f);
		model = translate(model, crossbows[i]->getPos());
		model = rotate(model, radians(90.0f * (crossbows[i]->getDir() + 1)), vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mv = view * model; // modle view matrix
		pProg.setUniform("MV", mv);
		pProg.setUniform("camPos", camera->getPosition());
		pProg.setUniform("Proj", projection);

		// update pass
		pProg.setUniform("Pass", 1);	

		crossbows[i]->updateParticles(&pProg);

		//render pass
		pProg.setUniform("Pass", 2);

		crossbows[i]->renderParticles(&pProg);
	}

	prog.use();
}

void SceneBasic_Uniform::createCloudQuad() {
	/////////////////// Create the VBO ////////////////////
	float verts[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
	};
	float tc[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

	// Create and populate the buffer objects
	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

	// Create and set-up the vertex array object
	glGenVertexArrays(1, &cloudQuad);
	glBindVertexArray(cloudQuad);

	//Set up the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	prog.setUniform("NoiseTex", 0);

	noiseTex = NoiseTex::generate2DTex(5.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glm::vec2 offset = glm::vec2(1.0f);
	prog.setUniform("offset", offset);
}