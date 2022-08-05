#include "camera.h"

void JCamera::Update()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

glm::mat4 JCamera::ProjectionMatrix()
{
    return glm::perspective(
        glm::radians(fov),
        aspect,
        nearClip,
        farClip
    );
}

glm::mat4 JCamera::ViewMatrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}