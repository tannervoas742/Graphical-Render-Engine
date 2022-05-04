#include "PCH.h"
#include "Renderer.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Number of samples per pixel for MSAA
unsigned int samples = 8;

OpenGLWindow* InitScreen(void)
{
	COUT("Render Thread Launch");

	if (GREGlobalSettings.RendererStarted == false) {
		COUT("Renderer not started yet. Will wait.");
		while (GREGlobalSettings.RendererStarted == false) {
			usleep(100);
		}
		COUT("Renderer started. Render thread woken up.");
	}

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Only use this if you don't have a framebuffer
	//glfwWindowHint(GLFW_SAMPLES, samples);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	OpenGLWindow* MyWindow = new OpenGLWindow();
	MyWindow->Init(width, height);
	//MyWindow.SetFullScreen(true);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	glViewport(0, 0, width, height);

	return MyWindow;
}

void RenderThreadLaunch(void)
{
	OpenGLWindow* MyWindow = InitScreen();

	SceneRenderer MySceneRenderer = SceneRenderer(width, height, samples);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Enables Multisampling
	glEnable(GL_MULTISAMPLE);

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	MyWindow->MainLoop(&MySceneRenderer);

	MySceneRenderer.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(MyWindow->GetWindow());
	delete MyWindow;
	// Terminate GLFW before ending the program
	glfwTerminate();
	GREGlobalSettings.RendererStarted = false;
	GREGlobalSettings.TerminateRenderer = true;
	return;
}

bool LaunchRenderer(void)
{
	COUT("Launching renderer process with following settings");
	LogGREGlobalSettings(&std::cout);
	HANDLE Result = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RenderThreadLaunch, NULL, NULL, NULL);
	if (!Result) {
		COUT("ERROR: Renderer thread failed to Launch!");
		TerminateRenderer();
		return false;
	}
	return true;
}

void TerminateRenderer(void)
{
	COUT("Initiating renderer termination with following settings");
	GREGlobalSettings.TerminateRenderer = true;
	GREGlobalSettings.RendererStarted = false;
	LogGREGlobalSettings(&std::cout);
}