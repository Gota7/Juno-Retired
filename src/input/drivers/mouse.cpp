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

void IDriverMouse::OnSroll(GLFWwindow* window, double xOffset, double yOffset)
{
    globalMouse->currInputs[MOUSE_SCROLL_X_POS] += xOffset;
    globalMouse->currInputs[MOUSE_SCROLL_Y_POS] += yOffset;
}

void IDriverMouse::UpdateMovement(IDriverMouseButtons mouseButton, float& xPos, float& yPos)
{
    if (currInputs[mouseButton] && prevInputs[mouseButton] != 0.0f) // Mouse has been down.
    {
        xPos = currInputs[MOUSE_X_MOVE];
        yPos = currInputs[MOUSE_Y_MOVE];
    }
    else
    {
        xPos = 0.0f;
        yPos = 0.0f;
    }
}

void IDriverMouse::SupportedButtons(std::vector<int>& supportedButtons)
{
    supportedButtons.push_back(MOUSE_LEFT_BUTTON);
    supportedButtons.push_back(MOUSE_RIGHT_BUTTON);
    supportedButtons.push_back(MOUSE_MIDDLE_BUTTON);
    for (int i = MOUSE_BUTTON_END; i < MOUSE_DRIVER_POS_END; i++)
    {
        supportedButtons.push_back(i);
    }
}

void IDriverMouse::RegisterCallbacks()
{
    glfwSetMouseButtonCallback(window, OnButtonPress);
    glfwSetScrollCallback(window, OnSroll);
}

void IDriverMouse::Update()
{
    currInputs[MOUSE_SCROLL_X_MOVE] = currInputs[MOUSE_SCROLL_X_POS] - prevInputs[MOUSE_SCROLL_X_POS];
    currInputs[MOUSE_SCROLL_Y_MOVE] = currInputs[MOUSE_SCROLL_Y_POS] - prevInputs[MOUSE_SCROLL_Y_POS];
    IDriver::Update();
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    currInputs[MOUSE_X_MOVE] = xPos - currInputs[MOUSE_X_POS];
    currInputs[MOUSE_Y_MOVE] = yPos - currInputs[MOUSE_Y_POS];
    currInputs[MOUSE_X_POS] = (float)xPos;
    currInputs[MOUSE_Y_POS] = (float)yPos; // TODO: CORRECTLY SCALE INPUT!
    UpdateMovement(MOUSE_LEFT_BUTTON, currInputs[MOUSE_X_MOVE_L], currInputs[MOUSE_Y_MOVE_L]);
    UpdateMovement(MOUSE_RIGHT_BUTTON, currInputs[MOUSE_X_MOVE_R], currInputs[MOUSE_Y_MOVE_R]);
    UpdateMovement(MOUSE_MIDDLE_BUTTON, currInputs[MOUSE_X_MOVE_M], currInputs[MOUSE_Y_MOVE_M]);
}