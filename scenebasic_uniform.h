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

#include "helper/objmesh.h"

class crossBow;

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;

    GLuint cubeTex; // skybox texture

    GLuint hdrFBO;
    GLuint quad;
    GLuint hdrTex, avgTex;

    std::unique_ptr<ObjMesh> crossbow;
    std::unique_ptr<ObjMesh> arrow;

    SkyBox sky;

    std::vector<crossBow*> crossbows;

    float tPrev;
    float angle;

    void setProgDefaults(GLSLProgram* cProg);

    void compile();

    void setMatrices(glm::mat4 model, GLSLProgram* cProg);

    void setUpFullScreenQuad();
    void setupFBO();
    void pass1();
    void pass2();
    void computeLogAveLuminance();
    void drawScene();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void updateCamera(int direction);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
