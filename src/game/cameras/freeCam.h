#pragma once

#include "../../jsystem.h"
#include <GLFW/glfw3.h>

// Free cam.
struct GFreeCam : JCamera
{

    // Pan the camera.
    void Pan(float x, float y);

    // Move the camera forward.
    void Forward(float amount);

    // Rotate the camera.
    void Rotate(float x, float y);

    // Vfunctions.
    virtual void Update() override;

};