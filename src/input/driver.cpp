#include "driver.h"

void IDriver::Init()
{
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

    // Save previous inputs.
    prevInputs = currInputs;

}