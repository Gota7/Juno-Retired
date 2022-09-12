#pragma once

#include <map>
#include <vector>

// Driver for handling input events.
struct IDriver
{
    std::map<int, float> currInputs; // Current inputs.
    std::map<int, float> prevInputs; // Previous inputs.

    // Get a list of supported buttons.
    virtual void SupportedButtons(std::vector<int>& supportedButtons) {}

    // Gather current inputs.
    virtual void GatherCurrInputs() {}

    // Initialize the driver.
    void Init();

    // Update the current driver.
    void Update();

};