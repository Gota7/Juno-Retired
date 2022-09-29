#include "freeCam.h"

#include <tracy/Tracy.hpp>

void GFreeCam::Pan(float x, float y)
{
    ZoneScopedN("GFreeCam::Pan");

    const float panSpeed = 0.015f;
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * panSpeed * x;
    //glm::vec3 relativeUp = glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront)); // (A x B) x C = -(C * B)A + (C * A)B.
    glm::vec3 relativeUp = -glm::dot(cameraFront, cameraUp) * cameraFront + glm::dot(cameraFront, cameraFront) * cameraUp;
    cameraPos += relativeUp * panSpeed * y;
    needsMatrixUpdate = true;
}

void GFreeCam::Forward(float amount)
{
    ZoneScopedN("GFreeCam::Forward");

    cameraPos += amount * cameraFront * 0.2f;
    needsMatrixUpdate = true;
}

void GFreeCam::Rotate(float x, float y)
{
    ZoneScopedN("GFreeCam::Rotate");

    const float rotSpeed = 0.1f;
    yaw -= x * rotSpeed;
    pitch += y * rotSpeed;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    needsMatrixUpdate = true;
}

void GFreeCam::Update()
{
    ZoneScopedN("GFreeCam::Update");

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

    // Mouse and rotation.
    float xRot = input->freeCamButtons.ButtonDown(FREECAM_BT_ROT_X_AN);
    float yRot = input->freeCamButtons.ButtonDown(FREECAM_BT_ROT_Y_AN);
    if (xRot != 0.0f || yRot != 0.0f) Rotate(xRot, yRot);

    // Forward/backward.
    float forwardBackward = input->freeCamButtons.ButtonDown(FREECAM_BT_FORWARD_BACKWARD_AN);
    if (forwardBackward != 0.0f) Forward(forwardBackward);

    // Update camera.
    JCamera::Update();

}