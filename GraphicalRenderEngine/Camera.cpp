#include "PCH.h"
#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    this->width = width;
    this->height = height;
    Position = position;
    RotationX = new float(0.0f);
    RotationY = new float(0.0f);
}

Camera::Camera(int width, int height, glm::vec3 position, glm::vec2 rotation)
{
    this->width = width;
    this->height = height;
    Position = position;
    RotationX = new float(rotation.x);
    RotationY = new float(rotation.y);
}

void Camera::updateMatrix(void)
{
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Calculates upcoming vertical change in the Orientation
    Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    Orientation = glm::rotate(Orientation, glm::radians(-1 * (*RotationX)), glm::normalize(glm::cross(Orientation, Up)));

    // Rotates the Orientation left and right
    Orientation = glm::rotate(Orientation, glm::radians(-1 * (*RotationY)), Up);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
    Inputs(window, 1.0f);
}

void Camera::Inputs(GLFWwindow* window, float TimeSinceLastFrame)
{
    //ImGuiIO& io = ImGui::GetIO();
    //if (!io.WantCaptureKeyboard) {
    if (true) {
        // Handles key inputs
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Position += speed * Orientation * TimeSinceLastFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Position += speed * -glm::normalize(glm::cross(Orientation, Up)) * TimeSinceLastFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Position += speed * -Orientation * TimeSinceLastFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Position += speed * glm::normalize(glm::cross(Orientation, Up)) * TimeSinceLastFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            Position += speed * Up * TimeSinceLastFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            Position += speed * -Up * TimeSinceLastFrame;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            speed = 2.0f;
        }
    }

    //if (!io.WantCaptureMouse) {
    if (true) {
        // Handles mouse inputs
        if (glfwGetWindowMonitor(window) != nullptr || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            // Hides mouse cursor
            //ImGui::SetMouseCursor(ImGuiMouseCursor_None);

            if (firstClick)
            {
                glfwSetCursorPos(window, (width / 2), (height / 2));
                firstClick = false;
            }

            // Stores the coordinates of the cursor
            double mouseX;
            double mouseY;
            // Fetches the coordinates of the cursor
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
            // and then "transforms" them into degrees 

            *RotationX += TimeSinceLastFrame * sensitivity * (float)(mouseY - (height / 2)) / height;
            *RotationY += TimeSinceLastFrame * sensitivity * (float)(mouseX - (width / 2)) / width;

            // Decides whether or not the next vertical Orientation is legal or not
            if (*RotationX >= 85.0f)
            {
                *RotationX = 85.0f;
            }
            else if (*RotationX <= -85.0f)
            {
                *RotationX = -85.0f;
            }

            // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
            glfwSetCursorPos(window, (width / 2), (height / 2));
        }
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            // Unhides cursor since camera is not looking around anymore
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // Makes sure the next time the camera looks around it doesn't jump
            firstClick = true;
        }

        if (glfwGetWindowMonitor(window) != nullptr || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            Position += speed * Orientation * TimeSinceLastFrame;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
            speed += 5.0 * TimeSinceLastFrame;
        }
    }
}

void Camera::SetAspectRatio(int Width, int Height)
{
    width = Width;
    height = Height;
}
