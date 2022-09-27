#include "gamepad.h"

IDriverGamepad* IDriverGamepad::globalGamepads[NUM_PLAYERS];

void IDriverGamepad::Init()
{
    int playerNum = 0;
    for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
    {
        if (globalGamepads[playerNum]->TryConnectJoystick(jid))
        {
            playerNum++;
            if (playerNum >= NUM_PLAYERS) return;
        }
    }
}

void IDriverGamepad::OnConnectDisconnect(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {

        // First scan for matching GUID.
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            if (globalGamepads[i]->lastGUID == glfwGetJoystickGUID(jid))
            {
                if (globalGamepads[i]->TryConnectJoystick(jid)) return;
            }
        }

        // Second scan for any spot available.
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            if (globalGamepads[i]->TryConnectJoystick(jid)) return;
        }

    }
    else if (event == GLFW_DISCONNECTED)
    {

        // Remove joysticks with JID.
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            if (globalGamepads[i]->connected && globalGamepads[i]->jid == jid) globalGamepads[i]->DisconnectJoystick();
        }

        // TODO: REMOVE JID JOYSTICKS FROM AVAILABILITY QUEUE.

        // TODO: SEARCH IF THERE ARE UNOCCUPIED JOYSTICKS AVAILABLE AND USE THEM! SMART IDEA WOULD BE TO HAVE A QUEUE OF AVAILABLE ONES.

    }
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
    for (int i = 0; i < axesCount; i++)
    {
        currInputs[i + AXES_START] = 0.0f;
    }
    for (int i = 0; i < buttonCount; i++)
    {
        currInputs[i + BUTTON_START] = 0.0f;
    }
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
    for (int i = 0; i < axesCount; i++)
    {
        currInputs.erase(i + AXES_START);
    }
    for (int i = 0; i < buttonCount; i++)
    {
        currInputs.erase(i + BUTTON_START);
    }
}

void IDriverGamepad::RegisterCallbacks()
{
    glfwSetJoystickCallback(OnConnectDisconnect);
}

void IDriverGamepad::Update()
{
    if (gamepadMode)
    {
        GLFWgamepadstate st;
        if (glfwGetGamepadState(jid, &st))
        {
            for (int i = 0; i < axesCount; i++)
            {
                currInputs[i + AXES_START] = st.axes[i];
            }
            for (int i = 0; i < buttonCount; i++)
            {
                currInputs[i + BUTTON_START] = (float)st.buttons[i];
            }
        }
    }
    else
    {
        const float* axes = glfwGetJoystickAxes(jid, &axesCount);
        for (int i = 0; i < axesCount; i++)
        {
            currInputs[i + AXES_START] = axes[i];
        }
        const unsigned char* buttons = glfwGetJoystickButtons(jid, &buttonCount);
        for (int i = 0; i < buttonCount; i++)
        {
            currInputs[i + BUTTON_START] = (float)buttons[i];
        }
    }
}