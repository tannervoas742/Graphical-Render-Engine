#include "PCH.h"
#include "SceneRenderer.h"

// Controls the gamma function
float gamma = 2.2f;

float rectangleVertices[] =
{
	//  Coords   // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

// Vertices for plane with texture
std::vector<Vertex> vertices =
{
	Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for plane with texture
std::vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3
};

SceneRenderer::SceneRenderer(int width, int height, int samples)
{
	Width = width;
	Height = height;
	Samples = samples;

    // Creates camera object
    SceneCamera = new Camera(Width, Height, glm::vec3(0.0f, 0.0f, 2.0f), glm::vec2(0.0f, 0.0f));
    //Sets some camera defaults. These will be driven by UIHandler later.
    SceneCamera->FOVdeg = 45.0f;
    SceneCamera->nearPlane = 0.1f;
    SceneCamera->farPlane = 100.0f;

    // Generates Shader object using shaders default.vert and default.frag
	// Generates shaders
	Shader* shaderProgram = new Shader("default.vert", "default.frag", "default.geom");
	Shader* framebufferProgram = new Shader("framebuffer.vert", "framebuffer.frag");
	Shader* blurProgram = new Shader("framebuffer.vert", "blur.frag");

	shaderProgram->StoreShader("default");
	framebufferProgram->StoreShader("framebuffer");
	blurProgram->StoreShader("blur");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	Shader::LoadedShaders["default"]->Activate();
	glUniform4f(glGetUniformLocation(Shader::LoadedShaders["default"]->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(Shader::LoadedShaders["default"]->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	Shader::LoadedShaders["framebuffer"]->Activate();
	glUniform1i(glGetUniformLocation(Shader::LoadedShaders["framebuffer"]->ID, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(Shader::LoadedShaders["framebuffer"]->ID, "bloomTexture"), 1);
	glUniform1f(glGetUniformLocation(Shader::LoadedShaders["framebuffer"]->ID, "gamma"), gamma);
	Shader::LoadedShaders["blur"]->Activate();
	glUniform1i(glGetUniformLocation(Shader::LoadedShaders["blur"]->ID, "screenTexture"), 0);


	// Prepare framebuffer rectangle VBO and VAO
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Use this to disable VSync (not advized)
	//glfwSwapInterval(0);

	if (!GenerateFrameBuffers()) {
		COUT("Failed to generate framebuffers.");
		GREGlobalSettings.TerminateRenderer = true;
		return;
	}

	Model* LavaRectangle = new Model("LavaRectangle.json");
	ActiveModels.push_back(LavaRectangle);
	//GREGlobalSettings.TerminateRenderer = true;

}

SceneRenderer::~SceneRenderer(void)
{
    delete SceneCamera;
}

bool SceneRenderer::GenerateFrameBuffers(void)
{

	TexType = GL_TEXTURE_2D;
	if (Samples > 1) { // MSAA not working
		//TexType = GL_TEXTURE_2D_MULTISAMPLE;
	}

	// Create Frame Buffer Object
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// Create Framebuffer Texture
	glGenTextures(1, &postProcessingTexture);
	glBindTexture(TexType, postProcessingTexture);
	glTexImage2D(TexType, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(TexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(TexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(TexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(TexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TexType, postProcessingTexture, 0);

	// Create Second Framebuffer Texture
	glGenTextures(1, &bloomTexture);
	glBindTexture(TexType, bloomTexture);
	glTexImage2D(TexType, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(TexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(TexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(TexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(TexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, TexType, bloomTexture, 0);

	// Tell OpenGL we need to draw to both attachments
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		COUT(std::format("Post-Processing Framebuffer error: {}", fboStatus));
		return false;
	}
		

	// Create Ping Pong Framebuffers for repetitive blurring
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(TexType, pingpongBuffer[i]);
		glTexImage2D(TexType, 0, GL_RGBA16F, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(TexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(TexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(TexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(TexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TexType, pingpongBuffer[i], 0);

		fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
			COUT(std::format("Ping-Pong Framebuffer error: {}", fboStatus));
			return false;
		}
	}
	return true;
}

void SceneRenderer::DeleteFrameBuffers(void)
{
	glDeleteFramebuffers(1, &postProcessingFBO);
	glDeleteTextures(1, &postProcessingTexture);
	glDeleteTextures(1, &bloomTexture);
	glDeleteFramebuffers(2, pingpongFBO);
	glDeleteTextures(2, pingpongBuffer);
}

void SceneRenderer::Render(GLFWwindow* Window, int NewWidth, int NewHeight)
{
	bool Resized = false;
	if (NewWidth != Width || NewHeight != Height)
	{
		Width = NewWidth;
		Height = NewHeight;
		COUT(std::format("Window resized: {}x{}", Width, Height));
		Resized = true;
		SceneCamera->SetAspectRatio(Width, Height);
		DeleteFrameBuffers();
		if (!GenerateFrameBuffers()) {
			COUT("Failed to re-generate framebuffers.");
			GREGlobalSettings.TerminateRenderer = true;
			return;
		}
	}

	// Updates counter and times
	prevTime = crntTime;
	crntTime = glfwGetTime();
	double timeDiff = crntTime - prevTime;
	double timeDiffDisplay = crntTime - prevDisplayTime;
	counter++;

	if (timeDiffDisplay >= 1.0 / 30.0 || Resized)
	{
		// Creates new title
		std::string FPS = std::to_string((1.0 / timeDiffDisplay) * counter);
		std::string ms = std::to_string((timeDiffDisplay / counter) * 1000);
		std::string width = std::to_string(Width);
		std::string height = std::to_string(Height);
		std::string newTitle = "Graphical Render Engine - " + FPS + "FPS / " + ms + "ms" + " - AR: " + width + "x" + height;
		glfwSetWindowTitle(Window, newTitle.c_str());

		// Resets times and counter
		prevDisplayTime = crntTime;
		counter = 0;

		// Use this if you have disabled VSync
		//SceneCamera.Inputs(window);
	}


	// Bind the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
	// Specify the color of the background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
	glEnable(GL_DEPTH_TEST);

	// Handles camera inputs (delete this if you have disabled VSync)
	SceneCamera->Inputs(Window, timeDiff);
	// Updates and exports the camera matrix to the Vertex Shader
	SceneCamera->updateMatrix();

	//TODO: Sort models by diffuse shader type, activate shader, load textures, and draw.
	Shader::LoadedShaders["default"]->Activate();
	for (int ModelIndex = 0; ModelIndex < ActiveModels.size(); ModelIndex++) {
		ActiveModels[ModelIndex]->GetTexture("normal")->Bind();
		glUniform1i(glGetUniformLocation(Shader::LoadedShaders["default"]->ID, "normal0"), 1);
		ActiveModels[ModelIndex]->GetTexture("displacement")->Bind();
		glUniform1i(glGetUniformLocation(Shader::LoadedShaders["default"]->ID, "displacement0"), 2);

		// Draw the normal model
		ActiveModels[ModelIndex]->Draw(*Shader::LoadedShaders["default"], *SceneCamera);
	}

	

	// Bounce the image data around to blur multiple times
	bool horizontal = true, first_iteration = true;
	// Amount of time to bounce the blur
	int amount = 2;
	Shader::LoadedShaders["blur"]->Activate();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		glUniform1i(glGetUniformLocation(Shader::LoadedShaders["blur"]->ID, "horizontal"), horizontal);

		// In the first bounc we want to get the data from the bloomTexture
		if (first_iteration)
		{
			glBindTexture(TexType, bloomTexture);
			first_iteration = false;
		}
		// Move the data between the pingPong textures
		else
		{
			glBindTexture(TexType, pingpongBuffer[!horizontal]);
		}

		// Render the image
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Switch between vertical and horizontal blurring
		horizontal = !horizontal;
	}


	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw the framebuffer rectangle
	Shader::LoadedShaders["framebuffer"]->Activate();
	glBindVertexArray(rectVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(TexType, postProcessingTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(TexType, pingpongBuffer[!horizontal]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void SceneRenderer::Delete(void)
{

	for (auto ShaderIter = Shader::LoadedShaders.begin(); ShaderIter != Shader::LoadedShaders.end(); ++ShaderIter) {
		Shader* ThisShader = ShaderIter->second;
		ThisShader->Delete();
	}
	Shader::LoadedShaders.clear();

	for (auto TextureIter = Texture::LoadedTextures.begin(); TextureIter != Texture::LoadedTextures.end(); ++TextureIter) {
		Texture* ThisTexture = TextureIter->second;
		ThisTexture->Delete();
	}
	Texture::LoadedTextures.clear();

	DeleteFrameBuffers();
}
