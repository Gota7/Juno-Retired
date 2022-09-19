#pragma once

#include "controller.h"

#include "../jsystem.h"

// Trigger an activation every X seconds.
template<typename T, size_t U>
struct IRepeater
{
    float timePeriod; // How much time to pass until event occurs.
    float lastTime; // Time since last update.
    IControllerAssignment<T, U>& assignment; // Assignment to poll.
    T button; // Button to pull.
    bool resetIfReleased = true; // Reset counter if released.

    // Constructor.
    IRepeater(IControllerAssignment<T, U>& assignment, T button) : assignment(assignment), button(button)
    {
        lastTime = (float)JFrame::currentFrame;
    }

    // Get the input value.
    float Poll()
    {
        float val = assignment.ButtonDown(button);
        if (val == 0.0f)
        {
            if (resetIfReleased) lastTime = (float)JFrame::currentFrame;
            return 0.0f;
        }
        float diff = (float)JFrame::currentFrame - lastTime;
        if (diff > timePeriod)
        {
            lastTime = JFrame::currentFrame + diff;
            return val;
        }
        return 0.0f;
    }

};