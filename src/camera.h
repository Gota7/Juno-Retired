#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "window.h"

// Camera structure.
struct JCamera
{
    glm::vec3 cameraPos;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float pitch = 0.0f;
    float yaw = -90.0f;
    float fov = 45.0f;
    float aspect = (float)SCR_WIDTH / SCR_HEIGHT;
    float nearClip = 0.1f;
    float farClip = 100.0f;

    void Update();
    glm::mat4 ProjectionMatrix();
    glm::mat4 ViewMatrix();
};