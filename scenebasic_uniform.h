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
#include "helper/plane.h"
#include "flag.h"

#include "helper/objmesh.h"

class crossBow;

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog, pProg;

    GLuint cubeTex; // skybox texture
    GLuint cloudQuad; // Quad to display clouds on
    GLuint noiseTex; // noise texture for clouds

    GLuint hdrFBO;
    GLuint quad;
    GLuint hdrTex, avgTex;

    // list of textures
    GLuint metalTex, metalNormal, rustTex, rustNormal, woodTex, woodNormal, smoke, particleTex, flagTex;

    std::unique_ptr<ObjMesh> crossbow;
    std::unique_ptr<ObjMesh> arrow;

    SkyBox sky;

    std::vector<crossBow*> crossbows;

    // position and direction of particle emitter
    glm::vec3 emitterPos, emitterDir;

    //particle buffers
    GLuint posBuf[2], velBuf[2], age[2];

    //particle VAOs
    GLuint particleArray[2];

    // Transform feedbacks
    GLuint feedback[2];

    GLuint drawBuf;

    std::vector<Flag*> flags;

    int nParticles;
    float cTime, particleLifetime, deltaT;

    float tPrev;
    float angle;

    time_t recordTime = 0;

    time_t startTime;

    bool homeScreen = true;
    vec3 homeScreenPos = vec3(2.0f, 0.9f, 1.5f);
    vec3 homeScreenFront = vec3(0.0f, 0.0f, -1.0f);
    vec3 GameStartPos = vec3(2.0f, 0.0f, 2.5f);

    void setProgDefaults(GLSLProgram* cProg);

    void compile();

    void setMatrices(glm::mat4 model, GLSLProgram* cProg);

    void setUpFullScreenQuad();
    void setupFBO();
    void pass1();
    void pass2();
    void computeLogAveLuminance();
    void drawScene();
    void createCloudQuad();

    void initBuffers();
    void renderParticles();

    void startGame();
    void endGame();

    void renderUI();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void updateCamera(int direction);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
