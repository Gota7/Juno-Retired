#include "driver.h"

#include <tracy/Tracy.hpp>

void IDriver::Init()
{
    ZoneScopedN("IDriver::Init");

    std::vector<int> supportedButtons;
    SupportedButtons(supportedButtons);
    for (auto button : supportedButtons)
    {
        currInputs[button] = 0.0f;
    }
    RegisterCallbacks();
}

void IDriver::Update()
{
    ZoneScopedN("IDriver::Update");

    // Save previous inputs.
    prevInputs = currInputs;

}