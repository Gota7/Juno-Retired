#pragma once

#include "../driver.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum IDriverMouseButtons
{
    MOUSE_LEFT_BUTTON = GLFW_MOUSE_BUTTON_LEFT,
    MOUSE_RIGHT_BUTTON = GLFW_MOUSE_BUTTON_RIGHT,
    MOUSE_MIDDLE_BUTTON = GLFW_MOUSE_BUTTON_MIDDLE,

    MOUSE_BUTTON_END
};

// Relative to screen units, so scale independent.
enum IDriverMousePositions
{
    MOUSE_X_POS = MOUSE_BUTTON_END,
    MOUSE_Y_POS,
    MOUSE_X_MOVE,
    MOUSE_Y_MOVE
};

// Mouse driver.
struct IDriverMouse : IDriver
{
    GLFWwindow* window; // Window.
    static IDriverMouse* globalMouse; // Global mouse driver.

    // Create a new mouse driver.
    IDriverMouse(GLFWwindow* window) : window(window) {}

    // Button press action.
    static void OnButtonPress(GLFWwindow* window, int button, int action, int mods);

    // Vfunctions.
    virtual void SupportedButtons(std::vector<int>& supportedButtons) override;
    virtual void RegisterCallbacks() override;
    virtual void Update() override;

};