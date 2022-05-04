#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
    // Stores the main vectors of the camera
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;

    float* RotationX;
    float* RotationY;

    // Stores the width and height of the window
    int width;
    int height;

    // Adjust the speed of the camera and it's sensitivity when looking around
    float speed = 2.0f;
    float sensitivity = 8000.0f;

    float FOVdeg;
    float nearPlane;
    float farPlane;

    // Camera constructor to set up initial values
    Camera(int width, int height, glm::vec3 position);
    Camera(int width, int height, glm::vec3 position, glm::vec2 rotation);

    // Updates the camera matrix to the Vertex Shader
    void updateMatrix(void);
    // Exports the camera matrix to a shader
    void Matrix(Shader& shader, const char* uniform);
    // Handles camera inputs
    void Inputs(GLFWwindow* window);
    void Inputs(GLFWwindow* window, float TimeSinceLastFrame);

    void SetAspectRatio(int Width, int Height);
protected:
    float TimeWithoutClick = 0.0f;
};
#endif