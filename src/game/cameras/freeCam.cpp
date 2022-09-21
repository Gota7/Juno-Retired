#include "freeCam.h"

void GFreeCam::Pan(float x, float y)
{
    const float panSpeed = 0.015f;
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * panSpeed * x;
    //glm::vec3 relativeUp = glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront)); // (A x B) x C = -(C * B)A + (C * A)B.
    glm::vec3 relativeUp = -glm::dot(cameraFront, cameraUp) * cameraFront + glm::dot(cameraFront, cameraFront) * cameraUp;
    cameraPos += relativeUp * panSpeed * y;
    needsMatrixUpdate = true;
}

void GFreeCam::Forward(float amount)
{
    cameraPos += amount * cameraFront * 0.2f;
    needsMatrixUpdate = true;
}

void GFreeCam::Rotate(float x, float y)
{
    const float rotSpeed = 0.1f;
    yaw -= x * rotSpeed;
    pitch += y * rotSpeed;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    needsMatrixUpdate = true;
}

// void GFreeCam::GLFWMouse(GLFWwindow* window, double xposIn, double yposIn)
// {
//     static float lastX;
//     static float lastY;
//     static bool leftWasPressed = false;
//     static bool rightWasPressed = false;
//     if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//     {
//         if (!leftWasPressed)
//         {
//             lastX = xposIn;
//             lastY = yposIn;
//             leftWasPressed = true;
//         }
//         Pan(xposIn - lastX, yposIn - lastY);
//         lastX = xposIn;
//         lastY = yposIn;
//     }
//     else if (leftWasPressed)
//     {
//         leftWasPressed = false;
//     }
//     if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
//     {
//         if (!rightWasPressed)
//         {
//             lastX = xposIn;
//             lastY = yposIn;
//             rightWasPressed = true;
//         }
//         Rotate(xposIn - lastX, yposIn - lastY);
//         lastX = xposIn;
//         lastY = yposIn;
//     }
//     else if (rightWasPressed)
//     {
//         rightWasPressed = false;
//     }
// }

void GFreeCam::Update()
{

    // Check for simple movement.
    float cameraSpeed = static_cast<float>(2.5 * JFrame::deltaTime);
    if (input->freeCamButtons.ButtonDown(FREECAM_BT_FORWARD))
    {
        cameraPos += cameraFront * cameraSpeed;
        needsMatrixUpdate = true;
    }
    if (input->freeCamButtons.ButtonDown(FREECAM_BT_BACKWARD))
    {
        cameraPos -= cameraFront * cameraSpeed;
        needsMatrixUpdate = true;
    }
    if (input->freeCamButtons.ButtonDown(FREECAM_BT_LEFT))
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        needsMatrixUpdate = true;
    }
    if (input->freeCamButtons.ButtonDown(FREECAM_BT_RIGHT))
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        needsMatrixUpdate = true;
    }
    if (input->freeCamButtons.ButtonDown(FREECAM_BT_UP))
    {
        cameraPos += cameraSpeed * cameraUp;
        needsMatrixUpdate = true;
    }
    if (input->freeCamButtons.ButtonDown(FREECAM_BT_DOWN))
    {
        cameraPos -= cameraSpeed * cameraUp;
        needsMatrixUpdate = true;
    }

    // Mouse and panning.
    float xPan = input->freeCamButtons.ButtonDown(FREECAM_BT_LEFT_RIGHT_AN);
    float yPan = input->freeCamButtons.ButtonDown(FREECAM_BT_UP_DOWN_AN);
    if (xPan != 0.0f || yPan != 0.0f) Pan(xPan, yPan);

    // Update camera.
    JCamera::Update();

}