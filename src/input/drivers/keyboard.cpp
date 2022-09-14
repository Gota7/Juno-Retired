#include "keyboard.h"

IDriverKeyboard* IDriverKeyboard::globalKeyboard;

void IDriverKeyboard::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        globalKeyboard->currInputs[key] = 1.0f;
    }
    else if (action == GLFW_RELEASE)
    {
        globalKeyboard->currInputs[key] = 0.0f;
    }
}

void IDriverKeyboard::SupportedButtons(std::vector<int>& supportedButtons)
{
    for (auto key : IDRIVER_KEYBOARDKEYS_ALL)
    {
        supportedButtons.push_back(key);
    }
}

void IDriverKeyboard::RegisterCallbacks()
{
    glfwSetKeyCallback(window, OnKeyPress);
}