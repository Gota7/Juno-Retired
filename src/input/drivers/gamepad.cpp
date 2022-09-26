#include "gamepad.h"

IDriverGamepad* IDriverGamepad::globalGamepads[NUM_PLAYERS];

IDriverGamepad::IDriverGamepad(GLFWwindow* window) : window(window)
{
}

void IDriverGamepad::OnConnectDisconnect(int jid, int event)
{
}

bool IDriverGamepad::TryConnectJoystick(int jid)
{
    if (connected || !glfwJoystickPresent(jid)) return false;
    if (glfwJoystickIsGamepad(jid))
    {
        gamepadMode = true;
        axesCount = GLFW_GAMEPAD_AXIS_LAST + 1;
        buttonCount = GLFW_GAMEPAD_BUTTON_LAST + 1;
        name = std::string(glfwGetGamepadName(jid));
    }
    else
    {
        gamepadMode = false;
        glfwGetJoystickAxes(jid, &axesCount);
        glfwGetJoystickButtons(jid, &buttonCount);
        name = std::string(glfwGetJoystickName(jid));
    }
    // TODO: SETUP MAP IN ADVANCE!
    this->jid = jid;
    connected = true;
    lastGUID = std::string(glfwGetJoystickGUID(jid));
    return true;
}

void IDriverGamepad::DisconnectJoystick()
{

    // We need to mark as disconnected and erase any current inputs.
    // Nonexistant values are inserted to input map when needed so we should be safe to eras.
    connected = false;
    // for (int i = 0; i < axesCount; i++)
    // {
    //     currInputs.erase(i + AXES_START);
    // }
    // for (int i = 0; i < buttonCount; i++)
    // {
    //     currInputs.erase(i + BUTTON_START);
    // }
    // for (int i = 0; i < hatCount; i++)
    // {
    //     currInputs.erase(i + HAT_START);
    // }
}

void IDriverGamepad::RegisterCallbacks()
{
    glfwSetJoystickCallback(OnConnectDisconnect);
}

void IDriverGamepad::Update()
{
}