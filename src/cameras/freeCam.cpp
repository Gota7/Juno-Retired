#include "freeCam.h"
#include "../frame.h"

void JFreeCam::Pan(float x, float y)
{
    const float panSpeed = 0.015f;
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * panSpeed * x;
    //glm::vec3 relativeUp = glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront)); // (A x B) x C = -(C * B)A + (C * A)B.
    glm::vec3 relativeUp = -glm::dot(cameraFront, cameraUp) * cameraFront + glm::dot(cameraFront, cameraFront) * cameraUp;
    cameraPos += relativeUp * panSpeed * y;
}

void JFreeCam::Forward(float amount)
{
    cameraPos += amount * cameraFront * 0.2f;
}

void JFreeCam::Rotate(float x, float y)
{
    const float rotSpeed = 0.1f;
    yaw -= x * rotSpeed;
    pitch += y * rotSpeed;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void JFreeCam::GLFWKeys(GLFWwindow* window)
{
    // Camera.
    float cameraSpeed = static_cast<float>(2.5 * JFrame::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

void JFreeCam::GLFWMouse(GLFWwindow* window, double xposIn, double yposIn)
{
    static float lastX;
    static float lastY;
    static bool leftWasPressed = false;
    static bool rightWasPressed = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!leftWasPressed)
        {
            lastX = xposIn;
            lastY = yposIn;
            leftWasPressed = true;
        }
        Pan(xposIn - lastX, yposIn - lastY);
        lastX = xposIn;
        lastY = yposIn;
    }
    else if (leftWasPressed)
    {
        leftWasPressed = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (!rightWasPressed)
        {
            lastX = xposIn;
            lastY = yposIn;
            rightWasPressed = true;
        }
        Rotate(xposIn - lastX, yposIn - lastY);
        lastX = xposIn;
        lastY = yposIn;
    }
    else if (rightWasPressed)
    {
        rightWasPressed = false;
    }
}