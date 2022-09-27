#pragma once

#include "../driver.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#define AXES_START 0
#define BUTTON_START 10000

// Axes for the gamepads.
enum IDriverGamepadAxes
{
    GAMEPAD_AXIS_LEFT_X = GLFW_GAMEPAD_AXIS_LEFT_X,
    GAMEPAD_AXIS_LEFT_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
    GAMEPAD_AXIS_RIGHT_X = GLFW_GAMEPAD_AXIS_RIGHT_X,
    GAMEPAD_AXIS_RIGHT_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,
    GAMEPAD_AXIS_LEFT_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
    GAMEPAD_AXIS_RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,

    GAMEPAD_AXIS_END
};

// Buttons for the gamepads.
enum IDriverGamepadButtons
{
    GAMEPAD_BUTTON_A = GLFW_GAMEPAD_BUTTON_A,
    GAMEPAD_BUTTON_B = GLFW_GAMEPAD_BUTTON_B,
    GAMEPAD_BUTTON_X = GLFW_GAMEPAD_BUTTON_X,
    GAMEPAD_BUTTON_Y = GLFW_GAMEPAD_BUTTON_Y,
    GAMEPAD_BUTTON_LEFT_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
    GAMEPAD_BUTTON_RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
    GAMEPAD_BUTTON_BACK = GLFW_GAMEPAD_BUTTON_BACK,
    GAMEPAD_BUTTON_START = GLFW_GAMEPAD_BUTTON_START,
    GAMEPAD_BUTTON_GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE,
    GAMEPAD_BUTTON_LEFT_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
    GAMEPAD_BUTTON_RIGHT_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    GAMEPAD_BUTTON_DPAD_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP,
    GAMEPAD_BUTTON_DPAD_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    GAMEPAD_BUTTON_DPAD_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    GAMEPAD_BUTTON_DPAD_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,

    GAMEPAD_BUTTON_END
};

// Gamepad driver.
struct IDriverGamepad : IDriver
{
    GLFWwindow* window; // Window.
    static IDriverGamepad* globalGamepads[NUM_PLAYERS]; // Global gamepad drivers.
    std::string name; // Name of the joystick or gamepad.
    std::string lastGUID = ""; // GUID of last connected joystick.
    int jid; // Joystick ID.
    int axesCount; // How many axes the joystick has.
    int buttonCount; // How many buttons the joystick has.
    bool gamepadMode; // If to treat the joystick as a gamepad instead.
    bool connected = false; // Nothing should work if gamepad off.

    // Create a gamepad driver.
    IDriverGamepad(GLFWwindow* window) : window(window) {}

    // Initialize all players.
    static void Init();

    // Joystick callback.
    static void OnConnectDisconnect(int jid, int event);

    // Switch players.
    void SwitchPlayerJoysticks(int playerA, int playerB);

    // Try connecting a new joystick.
    bool TryConnectJoystick(int jid);

    // Disconnect a joystick.
    void DisconnectJoystick();

    // Vfunctions.
    virtual void RegisterCallbacks() override;
    virtual void Update() override;

};