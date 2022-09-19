#include "mouse.h"

IDriverMouse* IDriverMouse::globalMouse;

void IDriverMouse::OnButtonPress(GLFWwindow* window, int button, int action, int mods)
{
    if (button > MOUSE_MIDDLE_BUTTON) return;
    if (action == GLFW_PRESS)
    {
        globalMouse->currInputs[button] = 1.0f;
    }
    else if (action == GLFW_RELEASE)
    {
        globalMouse->currInputs[button] = 0.0f;
    }
}

void IDriverMouse::SupportedButtons(std::vector<int>& supportedButtons)
{
    supportedButtons.push_back(MOUSE_LEFT_BUTTON);
    supportedButtons.push_back(MOUSE_RIGHT_BUTTON);
    supportedButtons.push_back(MOUSE_MIDDLE_BUTTON);
    supportedButtons.push_back(MOUSE_X_POS);
    supportedButtons.push_back(MOUSE_Y_POS);
    supportedButtons.push_back(MOUSE_X_MOVE);
    supportedButtons.push_back(MOUSE_Y_MOVE);
}

void IDriverMouse::RegisterCallbacks()
{
    glfwSetMouseButtonCallback(window, OnButtonPress);
}

void IDriverMouse::Update()
{
    IDriver::Update();
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    currInputs[MOUSE_X_MOVE] = xPos - currInputs[MOUSE_X_POS];
    currInputs[MOUSE_Y_MOVE] = yPos - currInputs[MOUSE_Y_POS];
    currInputs[MOUSE_X_POS] = (float)xPos;
    currInputs[MOUSE_Y_POS] = (float)yPos; // TODO: CORRECTLY SCALE INPUT!
}