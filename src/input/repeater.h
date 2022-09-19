#pragma once

#include "controller.h"

// Trigger an activation every X seconds.
struct IRepeater
{
    float timePeriod; // How much time to pass until event occurs.
    float lastTime; // Time since last update.

    // Constructor.
    IRepeater();

    // Get the input value.
    float Poll();

};