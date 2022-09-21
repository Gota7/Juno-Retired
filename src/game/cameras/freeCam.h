#pragma once

#include "../../jsystem.h"
#include "../../input.h"
#include <GLFW/glfw3.h>

// Free cam.
struct GFreeCam : JCamera
{
    IController* input;

    // Make a new free camera.
    GFreeCam(IController* input) : input(input) {}

    // Pan the camera.
    void Pan(float x, float y);

    // Move the camera forward.
    void Forward(float amount);

    // Rotate the camera.
    void Rotate(float x, float y);

    // Vfunctions.
    virtual void Update() override;

};