#include "camera.h"

#include <tracy/Tracy.hpp>

void JCamera::Update()
{
    ZoneScopedN("JCamera::Update");

    if (!needsMatrixUpdate) return;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    projection = glm::perspective(
        glm::radians(fov),
        aspect,
        nearClip,
        farClip
    );
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    needsMatrixUpdate = false;
}