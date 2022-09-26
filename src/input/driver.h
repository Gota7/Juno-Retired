#pragma once

#include <map>
#include <vector>

#define NUM_PLAYERS 4

// Driver for handling input events.
struct IDriver
{
    std::map<int, float> currInputs; // Current inputs.
    std::map<int, float> prevInputs; // Previous inputs.

    // Get a list of supported buttons.
    virtual void SupportedButtons(std::vector<int>& supportedButtons) {}

    // Register frame-wise callbacks needed.
    virtual void RegisterCallbacks() {}

    // Initialize the driver.
    void Init();

    // Update the current driver.
    virtual void Update();

};