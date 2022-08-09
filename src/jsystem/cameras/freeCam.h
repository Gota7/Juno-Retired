#pragma once

#include "../camera.h"
#include <GLFW/glfw3.h>

// Free cam.
struct JFreeCam : JCamera
{

    // Pan the camera.
    void Pan(float x, float y);

    // Move the camera forward.
    void Forward(float amount);

    // Rotate the camera.
    void Rotate(float x, float y);

    // GLFW keys.
    void GLFWKeys(GLFWwindow* window);

    // GLFW mouse.
    void GLFWMouse(GLFWwindow* window, double xposIn, double yposIn);

};