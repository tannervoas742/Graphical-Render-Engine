#ifndef _SCENE_RENDERER_CLASS_H
#define _SCENE_RENDERER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "GREGlobals.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "Camera.h"
#include "ShaderClass.h"
#include "Model.h"
#include "ModelInstance.h"

class SceneRenderer
{
public:
    SceneRenderer(int width, int height, int samples);
    ~SceneRenderer();

    Camera* GetSceneCamera(void) { return SceneCamera; }

    void Render(GLFWwindow* Window, int NewWidth, int NewHeight);
    void Delete(void);
protected:
    bool GenerateFrameBuffers(void);
    void DeleteFrameBuffers(void);

    Camera* SceneCamera = NULL;
    std::vector<ModelInstance*> ActiveModels;

    double prevTime, crntTime, prevDisplayTime;
    double startTime = -1.0f;
    int counter;

    int Width = 1200;
    int Height = 800;
    int Samples = 1;

    GLenum TexType = GL_TEXTURE_2D;
    
    unsigned int rectVAO, rectVBO;
    unsigned int postProcessingFBO;
    unsigned int postProcessingTexture;
    unsigned int bloomTexture;
    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffer[2];

};
#endif