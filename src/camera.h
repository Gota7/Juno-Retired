#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "window.h"

// Camera structure.
struct JCamera
{
    glm::vec3 cameraPos; // Base position.
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Facing dir.
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector.
    float pitch = 0.0f; // Around X axis.
    float yaw = -90.0f; // Around Y axis.
    float fov = 45.0f; // Field of view degrees.
    float aspect = (float)SCR_WIDTH / SCR_HEIGHT;
    float nearClip = 0.1f;
    float farClip = 100.0f;

    // Update the camera front.
    void Update();

    // Get the projection matrix.
    glm::mat4 ProjectionMatrix();

    // Get the view matrix.
    glm::mat4 ViewMatrix();

};