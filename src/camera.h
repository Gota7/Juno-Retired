#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

// Camera structure.
struct JCamera
{
    glm::vec3 cameraPos;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 Matrix();
};