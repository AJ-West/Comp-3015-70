#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include <glm/gtc/matrix_transform.hpp>

#include "media/texture/texture.h"

#include "global.h"

using glm::vec3;

using glm::mat4;


struct crossBow {
	int x = 0;
	int y = 0;
	int z = 0;
	float rotation = 90.0f;

	//int rotateDelay = 6;
	int delayCount = 0;

	void updateRotation() {
		//delayCount++;
		//if (rotateDelay == delayCount) {
			//delayCount = 0;
		rotation -= 1.0f;
		if (rotation == 0.0f) {
			rotation = 90;
		}
		//}
	}
} crossBowStruct;

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30), sky(100.0f) {
	tPrev = 0;
	angle = 0;
}

void SceneBasic_Uniform::initScene()
{
    compile();
	glEnable(GL_DEPTH_TEST);
	model1 = mat4(1.0f);

	//model1 = glm::rotate(model1, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
	//model1 = glm::rotate(model1, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));
	model1 = translate(model1, vec3(2.0f, 0.0f, 0.0f));
	model1 = scale(model1, vec3(0.025f, 0.025f, 0.025f));

	model2 = mat4(1.0f);

	model2 = glm::rotate(model2, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	model2 = scale(model2, vec3(0.025f, 0.025f, 0.025f));
	

	view = lookAt(camera->getPosition(), camera->getPosition() + camera->getFront(), camera->getCameraUp());

	projection = mat4(1.0f);

	prog.use();

	setupFBO();
	setUpFullScreenQuad();

	prog.setUniform("Model", model1);

	setProgDefaults(&prog);

	GLuint metalTex = Texture::loadTexture("media/texture/metal/metal.png");
	GLuint metalNormal = Texture::loadTexture("media/texture/metal/metal_normalMap.png");
	GLuint rustTex = Texture::loadTexture("media/texture/rust/rust.png");
	GLuint rustNormal = Texture::loadTexture("media/texture/rust/rust_normalMap.png");
	GLuint woodTex = Texture::loadTexture("media/texture/wood/wood.png");
	GLuint woodNormal = Texture::loadTexture("media/texture/wood/wood_normalMap.png");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metalTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metalNormal);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, rustTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, rustNormal);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, woodTex);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, woodNormal);

	//ogre = ObjMesh::load("media/texture/bs_ears.obj", false, true);
	arrow = ObjMesh::load("media/models/arrow.obj", false, true);
	crossbow = ObjMesh::load("media/models/crossbow.obj", false, true);

	skyProg.use();

	skyModel = mat4(1.0f);

	GLuint cubeTex = Texture::loadHdrCubeMap("media/texture/mountainsCubeMap/skybox");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	prog.use();
}

void SceneBasic_Uniform::setUpFullScreenQuad() {
	GLfloat verts[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	};
	GLfloat tc[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	//Set up the buffers
	unsigned int handle[2];
	glGenBuffers(2, handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

	//Sets up the vertex array object
	glGenVertexArrays(1, &quad);
	glBindVertexArray(quad);
	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); //Vertex position
	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2); //Texture coords
	glBindVertexArray(0);
}

void SceneBasic_Uniform::setProgDefaults(GLSLProgram* cProg) {
	float x, z;
	for (int i = 0; i < 3; i++) {
		std::stringstream name;
		name << "Lights[" << i << "].Position";
		x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
		cProg->setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
	}
	cProg->setUniform("Lights[0].Ld", vec3(5.0f, 5.0f, 5.0f));
	cProg->setUniform("Lights[0].La", vec3(0.2f, 0.2f, 0.2f));
	cProg->setUniform("Lights[0].Ls", vec3(5.0f, 5.0f, 5.0f));

	/*cProg->setUniform("Lights[0].Ld", vec3(0.0f, 0.0f, 0.8f));
	cProg->setUniform("Lights[0].La", vec3(0.0f, 0.0f, 0.2f));
	cProg->setUniform("Lights[0].Ls", vec3(0.0f, 0.0f, 0.8f));

	cProg->setUniform("Lights[1].Ld", vec3(0.0f, 0.8f, 0.0f));
	cProg->setUniform("Lights[1].La", vec3(0.0f, 0.2f, 0.0f));
	cProg->setUniform("Lights[1].Ls", vec3(0.0f, 0.8f, 0.0f));

	cProg->setUniform("Lights[2].Ld", vec3(0.8f, 0.0f, 0.0f));
	cProg->setUniform("Lights[2].La", vec3(0.2f, 0.0f, 0.0f));
	cProg->setUniform("Lights[2].Ls", vec3(0.8f, 0.0f, 0.0f));*/

	cProg->setUniform("Fog.MaxDist", 10.0f);
	cProg->setUniform("Fog.MinDist", 0.0f);
	cProg->setUniform("Fog.Colour", vec3(0.0f, 0.0f, 0.0f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		skyProg.compileShader("shader/skybox.vert");
		skyProg.compileShader("shader/skybox.frag");
		prog.link();
		skyProg.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	view = lookAt(camera->getPosition(), camera->getPosition() + camera->getFront(), camera->getCameraUp());

	//updating light position	

	angle += 0.5f * (t - tPrev);

	tPrev = t;

	if (angle > glm::two_pi<float>()) {
		angle -= glm::two_pi<float>();
	}

	crossBowStruct.updateRotation();
}

void SceneBasic_Uniform::updateCamera(int direction) {
	if (direction == 5) { camera->togglePaused(); }
	camera->updatePosition(direction);
	view = lookAt(camera->getPosition(), camera->getPosition() + camera->getFront(), camera->getCameraUp());
}

void SceneBasic_Uniform::render()
{
	pass1();
	computeLogAveLuminance();
	pass2();
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

void SceneBasic_Uniform::pass1() {
	prog.setUniform("Pass", 1);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	projection = glm::perspective(glm::radians(70.0f), (float)width / height, 0.01f, 100.0f);
	drawScene();
}

void SceneBasic_Uniform::pass2() {
	prog.setUniform("Pass", 2);
	//Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	view = mat4(1.0f);
	projection = mat4(1.0f);
	setMatrices(mat4(1.0f), &prog);

	//Render the quad
	glBindVertexArray(quad);
	glDrawArrays(GL_TRIANGLES, 0,6);
}

void SceneBasic_Uniform::computeLogAveLuminance() {
	int size = width * height;
	std::vector<GLfloat> texData(size * 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData.data());
	float sum = 0.0f;
	for (int i = 0; i < size; i++) {
		float lum = glm::dot(vec3(texData[i * 3 + 0], texData[i * 3 + 1], texData[i * 3 + 2]), vec3(0.2126f, 0.7152f, 0.0722f));
		sum += logf(lum + 0.00001f);
	}
	prog.setUniform("AveLum", expf(sum / size));
}

void SceneBasic_Uniform::drawScene() {
	skyProg.use();
	skyModel = mat4(1.0f);
	skyProg.setUniform("MVP", projection * view * skyModel);
	sky.render();

	prog.use();

	float x, z;
	for (int i = 0; i < 3; i++) {
		std::stringstream name;
		name << "Lights[" << i << "].Position";
		x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
		prog.setUniform(name.str().c_str(), view * glm::vec4(x * cos(angle), 1.2f, (z + 1.0f) * sin(angle), 1.0f));
	}

	setMatrices(model1, &prog);
	prog.setUniform("Model", model1);
	prog.setUniform("arrow", true);
	arrow->render();
	//torus.render();


	model2 = mat4(1.0f);

	model2 = glm::rotate(model2, glm::radians(crossBowStruct.rotation), vec3(1.0f, 0.0f, 0.0f));
	//model2 = translate(model2, vec3(2.0f, 0.0f, 0.0f));
	model2 = scale(model2, vec3(0.025f, 0.025f, 0.025f));
	setMatrices(model2, &prog);
	prog.setUniform("Model", model2);
	prog.setUniform("arrow", false);
	crossbow->render();
}