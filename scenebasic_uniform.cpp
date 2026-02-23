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

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30), cube(1.0f), sky(100.0f) {
	tPrev = 0;
	angle = 0;
}

void SceneBasic_Uniform::initScene()
{
    compile();
	glEnable(GL_DEPTH_TEST);
	model1 = mat4(1.0f);

	model1 = glm::rotate(model1, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
	model1 = glm::rotate(model1, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));

	model2 = mat4(1.0f);

	model2 = translate(model2, vec3(2.0f, 0.0f, 0.0f));

	view = lookAt(camera->getPosition(), camera->getPosition() + camera->getFront(), camera->getCameraUp());

	projection = mat4(1.0f);

	prog.use();

	prog.setUniform("Model", model1);

	setProgDefaults(&prog);

	GLuint brickID = Texture::loadTexture("media/texture/brick1.jpg");
	GLuint mossID = Texture::loadTexture("media/texture/moss.png");
	GLuint texture = Texture::loadTexture("media/texture/ogre_diffuse.png");
	GLuint normalMap = Texture::loadTexture("media/texture/ogre_normalmap.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mossID);

	normalProg.use();

	ogre = ObjMesh::load("media/texture/bs_ears.obj", false, true);

	setProgDefaults(&normalProg);
	
	//GLuint textureNormal = Texture::loadTexture("media/texture/ogre_diffuse.png");
	//GLuint normalMap = Texture::loadTexture("media/texture/ogre_normalmap.png");

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMap);

	skyProg.use();

	skyModel = mat4(1.0f);

	GLuint cubeTex = Texture::loadHdrCubeMap("media/texture/pisa-hdr/pisa");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
}

void SceneBasic_Uniform::setProgDefaults(GLSLProgram* cProg) {
	float x, z;
	for (int i = 0; i < 1; i++) {
		std::stringstream name;
		name << "Lights[" << i << "].Position";
		x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
		cProg->setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
	}
	cProg->setUniform("Lights[0].Ld", vec3(0.8f, 0.8f, 0.8f));
	cProg->setUniform("Lights[0].La", vec3(0.2f, 0.2f, 0.2f));
	cProg->setUniform("Lights[0].Ls", vec3(0.8f, 0.8f, 0.8f));

	/*cProg->setUniform("Lights[0].Ld", vec3(0.0f, 0.0f, 0.8f));
	cProg->setUniform("Lights[0].La", vec3(0.0f, 0.0f, 0.2f));
	cProg->setUniform("Lights[0].Ls", vec3(0.0f, 0.0f, 0.8f));

	cProg->setUniform("Lights[1].Ld", vec3(0.0f, 0.8f, 0.0f));
	cProg->setUniform("Lights[1].La", vec3(0.0f, 0.2f, 0.0f));
	cProg->setUniform("Lights[1].Ls", vec3(0.0f, 0.8f, 0.0f));

	cProg->setUniform("Lights[2].Ld", vec3(0.8f, 0.0f, 0.0f));
	cProg->setUniform("Lights[2].La", vec3(0.2f, 0.0f, 0.0f));
	cProg->.setUniform("Lights[2].Ls", vec3(0.8f, 0.0f, 0.0f));*/

	cProg->setUniform("Fog.MaxDist", 5.0f);
	cProg->setUniform("Fog.MinDist", 1.0f);
	cProg->setUniform("Fog.Colour", vec3(0.2f, 0.2f, 0.2f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		normalProg.compileShader("shader/normal.vert");
		normalProg.compileShader("shader/normal.frag");
		skyProg.compileShader("shader/skybox.vert");
		skyProg.compileShader("shader/skybox.frag");
		prog.link();
		normalProg.link();
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
}

void SceneBasic_Uniform::updateCamera(int direction) {
	camera->updatePosition(direction);
	view = lookAt(camera->getPosition(), camera->getPosition() + camera->getFront(), camera->getCameraUp());
}

void SceneBasic_Uniform::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setting colour to same as fog colour
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

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
		prog.setUniform(name.str().c_str(), view * glm::vec4(x *cos(angle), 1.2f, (z + 1.0f)*sin(angle), 1.0f));
	}

	setMatrices(model1, &prog);
	prog.setUniform("Model", model1);
	torus.render();

	normalProg.use();
	setMatrices(model2, &normalProg);
	prog.setUniform("Model", model2);
	ogre->render();
	//cube.render();
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices(mat4 model, GLSLProgram* cProg) {
	mat4 mv = view * model;

	cProg->setUniform("camPos", camera->getPosition());
	
	cProg->setUniform("ModelViewMatrix", mv);

	cProg->setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

	cProg->setUniform("MVP", projection * mv);
}
