#pragma once
#include "global.h"
#include "media/texture/texture.h"
#include "helper/plane.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
/*
class Flag {
private:
	vec3 pos;
	vec3 rotation;

	Plane plane;

	//should add time to global variables
	float time = 0;

public:
	Flag(vec3 position, float xSize, float zSize, int xDivs, int zDivs, vec3 fRotation) :pos(position), plane(xSize, zSize, xDivs, zDivs), rotation(fRotation) {

	}

	void render(GLSLProgram* prog){
        prog.setUniform("Time", time);

        prog.setUniform("Material.Kd", 0.2f, 0.2f, 0.2f);
        prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
        prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
        prog.setUniform("Material.Shininess", 1.0f);

        model = mat4(1.0f);
        model = glm::rotate(model, glm::radians(50.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices();
        plane.render();
	}

    void setMatrices()
    {
        glm::mat4 mv = view * model; // modle view matrix
        prog.setUniform("ModelViewMatrix", mv);
        prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        prog.setUniform("MVP", projection * mv);
    }
};*/