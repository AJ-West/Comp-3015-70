#pragma once
#include "global.h"
#include "helper/plane.h"

class Flag {
private:
	vec3 pos;
	vec3 rotation;

	Plane plane = Plane(16.0f, 10.0f, 200, 2);

	//should add time to global variables
	float time = 0;

public:
	Flag(vec3 position, vec3 fRotation) :pos(position), rotation(fRotation) {

	}

	void render(GLSLProgram* prog, glm::mat4 model, glm::mat4 view, glm::mat4 projection, float cTime){
        prog->setUniform("Time", cTime);

        prog->setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
        prog->setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
        prog->setUniform("Material.Ka", 1.0f, 1.0f, 1.0f);
        prog->setUniform("Material.Shininess", 1.0f);

        model = mat4(1.0f);
        model = translate(model, pos);
        model = glm::rotate(model, glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), vec3(0.0f, 0.0f, 1.0f));
        //model = glm::rotate(model, glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
        model = scale(model, vec3(0.15f, 0.15f, 0.15f));
        setMatrices(prog, model, view, projection);
        plane.render();
	}

    void setMatrices(GLSLProgram* prog, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        glm::mat4 mv = view * model; // modle view matrix
        prog->setUniform("ModelViewMatrix", mv);
        prog->setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        prog->setUniform("MVP", projection * mv);
    }
};