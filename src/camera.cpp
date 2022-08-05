#include "camera.h"

glm::mat4 JCamera::Matrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}