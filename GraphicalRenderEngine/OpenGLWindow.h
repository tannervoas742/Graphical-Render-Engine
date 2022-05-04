#ifndef _OPEN_GL_WINDOPW_CLASS_H
#define _OPEN_GL_WINDOPW_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "GREGlobals.h"
#include <array>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "SceneRenderer.h"

class OpenGLWindow
{
private:

    std::array< int, 2 > _wndPos{ 0, 0 };
    std::array< int, 2 > _wndSize{ 0, 0 };
    std::array< int, 2 > _vpSize{ 0, 0 };

    GLFWwindow* _wnd = nullptr;
    GLFWmonitor* _monitor = nullptr;

    void Resize(int cx, int cy);


public:
    bool _updateViewport = true;

    int Init(int width, int height);
    static void CallbackResize(GLFWwindow* window, int cx, int cy);
    void MainLoop(SceneRenderer* ThisSceneRender);
    bool IsFullscreen(void);
    void SetFullScreen(bool fullscreen);
    GLFWwindow* GetWindow(void);
};
#endif