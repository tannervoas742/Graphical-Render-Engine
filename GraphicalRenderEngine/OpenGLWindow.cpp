#include "PCH.h"
#include "OpenGLWindow.h"

int OpenGLWindow::Init(int width, int height)
{

    // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
    _wnd = glfwCreateWindow(width, height, "Graphical Render Engine", NULL, NULL);
    // Error check if the window fails to create
    if (_wnd == NULL)
    {
        COUT("Failed to create GLFW window");
        glfwTerminate();
        GREGlobalSettings.TerminateRenderer = true;
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(_wnd);

    glfwSetWindowUserPointer(_wnd, this);
    glfwSetWindowSizeCallback(_wnd, OpenGLWindow::CallbackResize);

    _monitor = glfwGetPrimaryMonitor();
    glfwGetWindowSize(_wnd, &_wndSize[0], &_wndSize[1]);
    glfwGetWindowPos(_wnd, &_wndPos[0], &_wndPos[1]);
    _updateViewport = true;

    return 0;
}

void OpenGLWindow::CallbackResize(GLFWwindow* window, int cx, int cy)
{
    void* ptr = glfwGetWindowUserPointer(window);
    if (OpenGLWindow* wndPtr = static_cast<OpenGLWindow*>(ptr)) {
        wndPtr->Resize(cx, cy);
    }
}

void OpenGLWindow::Resize(int cx, int cy)
{
    _updateViewport = true;
}

void OpenGLWindow::MainLoop(SceneRenderer* ThisSceneRender)
{

    while (!glfwWindowShouldClose(_wnd))
    {

        if (glfwGetKey(_wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(_wnd, GL_TRUE);
        }

        if (_updateViewport == true)
        {
            glfwGetFramebufferSize(_wnd, &_vpSize[0], &_vpSize[1]);
            glViewport(0, 0, _vpSize[0], _vpSize[1]);
            _updateViewport = false;
        }


        ThisSceneRender->Render(_wnd, _vpSize[0], _vpSize[1]);

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(_wnd);
        // Take care of all GLFW events
        glfwPollEvents();

		if (GREGlobalSettings.RendererStarted == false || GREGlobalSettings.TerminateRenderer == true) {
			COUT("Renderer loop is halting");
			break;
		}

    }
}

bool OpenGLWindow::IsFullscreen(void)
{
    return glfwGetWindowMonitor(_wnd) != nullptr;
}

void OpenGLWindow::SetFullScreen(bool fullscreen)
{
    if (IsFullscreen() == fullscreen)
        return;

    if (fullscreen)
    {
        // backup window position and window size
        glfwGetWindowPos(_wnd, &_wndPos[0], &_wndPos[1]);
        glfwGetWindowSize(_wnd, &_wndSize[0], &_wndSize[1]);

        // get resolution of monitor
        const GLFWvidmode* mode = glfwGetVideoMode(_monitor);

        // switch to full screen
        glfwSetWindowMonitor(_wnd, _monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        // restore last window size and position
        glfwSetWindowMonitor(_wnd, nullptr, _wndPos[0], _wndPos[1], _wndSize[0], _wndSize[1], 0);
    }

    _updateViewport = true;
}

GLFWwindow* OpenGLWindow::GetWindow(void)
{
    return _wnd;
}
