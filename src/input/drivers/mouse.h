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
    MOUSE_Y_MOVE,
    MOUSE_X_MOVE_L, // Move with left mouse button down.
    MOUSE_Y_MOVE_L,
    MOUSE_X_MOVE_R,
    MOUSE_Y_MOVE_R,
    MOUSE_X_MOVE_M,
    MOUSE_Y_MOVE_M,
    MOUSE_SCROLL_X_POS,
    MOUSE_SCROLL_Y_POS,
    MOUSE_SCROLL_X_MOVE,
    MOUSE_SCROLL_Y_MOVE,

    MOUSE_DRIVER_POS_END
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

    // Scroll wheel action.
    static void OnSroll(GLFWwindow* window, double xOffset, double yOffset);

    // Update movement.
    void UpdateMovement(IDriverMouseButtons mouseButton, float& xPos, float& yPos);

    // Vfunctions.
    virtual void SupportedButtons(std::vector<int>& supportedButtons) override;
    virtual void RegisterCallbacks() override;
    virtual void Update() override;

};