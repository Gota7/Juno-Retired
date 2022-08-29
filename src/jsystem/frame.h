#pragma once

#include <GLFW/glfw3.h>

// Time keeper.
struct JFrame
{
    static double lastFrame;
    static double currentFrame;
    static double deltaTime;
    static double lastSecond;
    static unsigned int targetFps;
    static double targetTime;
    static int fps;
    static int frameCounter;
    static float aspect;

    // Set FPS. Runs unlimited otherwise.
    static void SetFPS(unsigned int fps);

    // Update time.
    static void Update();

};