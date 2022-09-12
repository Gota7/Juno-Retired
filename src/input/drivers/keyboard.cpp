#include "keyboard.h"

void IDriverKeyboard::SupportedButtons(std::vector<int>& supportedButtons)
{
    for (auto key : IDRIVER_KEYBOARDKEYS_ALL)
    {
        supportedButtons.push_back(key);
    }
}

void IDriverKeyboard::GatherCurrInputs()
{
    for (auto key : IDRIVER_KEYBOARDKEYS_ALL)
    {
        currInputs[key] = (float)glfwGetKey(window, key);
    }
}