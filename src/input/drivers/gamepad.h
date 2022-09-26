#pragma once

#include "../driver.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#define AXES_START 0
#define BUTTON_START 10000

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
    IDriverGamepad(GLFWwindow* window);

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