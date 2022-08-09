#pragma once

#include <GLFW/glfw3.h>

// Time keeper.
struct JFrame
{
    static double lastFrame;
    static double currentFrame;
    static float deltaTime;

    // Update time.
    static void Update();
};