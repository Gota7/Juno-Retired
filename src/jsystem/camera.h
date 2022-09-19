#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "window.h"

// Camera structure.
struct JCamera
{
    glm::vec3 cameraPos = glm::vec3(0.0f); // Base position.
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Facing dir.
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector.
    glm::mat4 projection; // Last projection matrix.
    glm::mat4 view; // Last view matrix.
    float pitch = 0.0f; // Around X axis.
    float yaw = -90.0f; // Around Y axis.
    float fov = 45.0f; // Field of view degrees.
    float aspect = (float)SCR_WIDTH / SCR_HEIGHT;
    float nearClip = 0.1f;
    float farClip = 100.0f;
    bool needsMatrixUpdate = true;

    // Update the camera front.
    virtual void Update();

};