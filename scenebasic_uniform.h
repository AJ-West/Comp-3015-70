#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "helper/torus.h"
#include "media/texture/cube.h"
#include "helper/skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    GLSLProgram skyProg;

    Torus torus;

    Cube cube;

    SkyBox sky;

    float tPrev;
    float angle;

    //Camera camera;

    void compile();

    void setMatrices(glm::mat4 model);

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void updateCamera(int direction);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
