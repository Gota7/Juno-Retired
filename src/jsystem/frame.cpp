#include "frame.h"
#include <iostream>
#include <tracy/Tracy.hpp>

double JFrame::currentFrame = 0; // Current time in seconds.
double JFrame::lastFrame = 0; // Last frame in seconds.
double JFrame::deltaTime = 0; // Time between last frame and this one.
double JFrame::lastSecond = 0; // Last frame that was the seconds count.
unsigned int JFrame::targetFps = 60; // Target frames per seconds.
double JFrame::targetTime = 1.0 / 60; // Target time for each frame.
int JFrame::fps = 60; // Current FPS.
int JFrame::frameCounter = 0; // Frame counter for each second.
float JFrame::aspect = (float)16 / 9; // Aspect ratio of the game.

#define SUPPORT_BUSY_WAIT_LOOP true

void WaitTime(double seconds)
{
#if defined(SUPPORT_BUSY_WAIT_LOOP) || defined(SUPPORT_PARTIALBUSY_WAIT_LOOP)
    double destinationTime = glfwGetTime() + seconds;
#endif

#if defined(SUPPORT_BUSY_WAIT_LOOP)
    while (glfwGetTime() < destinationTime) { }
#else
    #if defined(SUPPORT_PARTIALBUSY_WAIT_LOOP)
        double sleepSeconds = seconds - seconds*0.05;  // NOTE: We reserve a percentage of the time for busy waiting
    #else
        double sleepSeconds = seconds;
    #endif

    // System halt functions
    #if defined(_WIN32)
        Sleep((unsigned long)(sleepSeconds*1000.0));
    #endif
    #if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__EMSCRIPTEN__)
        struct timespec req = { 0 };
        time_t sec = sleepSeconds;
        long nsec = (sleepSeconds - sec)*1000000000L;
        req.tv_sec = sec;
        req.tv_nsec = nsec;

        // NOTE: Use nanosleep() on Unix platforms... usleep() it's deprecated.
        while (nanosleep(&req, &req) == -1) continue;
    #endif
    #if defined(__APPLE__)
        usleep(sleepSeconds*1000000.0);
    #endif

    #if defined(SUPPORT_PARTIALBUSY_WAIT_LOOP)
        while (glfwGetTime() < destinationTime) { }
    #endif
#endif
}

void JFrame::SetFPS(unsigned int fps)
{
    ZoneScopedN("JFrame::SetFPS");
    if (fps < 1) targetTime = 0;
    else targetTime = 1.0 / fps;
}

void JFrame::Update()
{
    lastFrame = currentFrame;
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    if (deltaTime < targetTime)
    {
        //WaitTime(targetTime - deltaTime);
    }
    frameCounter++;
    if (currentFrame - lastSecond >= 1)
    {
        fps = frameCounter;
        frameCounter = 0;
        lastSecond = currentFrame;
        std::cout << "FPS: " << fps << std::endl;
    }
    FrameMark;
}