#pragma once

#include "../../input.h"
#include "../../jsystem.h"

// Follow camera.
struct GFollowCam : JCamera
{
    IController* input;

    // Make a new follow camera.
    GFollowCam(IController* input) : input(input) {}

    // Vfunctions.
    virtual void Update() override;

};