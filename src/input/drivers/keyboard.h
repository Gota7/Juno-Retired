#pragma once

#include "../driver.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum IDriverKeyboardKeys
{
    KEYBOARD_SPACE = GLFW_KEY_SPACE,
    KEYBOARD_APOSTROPHE = GLFW_KEY_APOSTROPHE,
    KEYBOARD_COMMA = GLFW_KEY_COMMA,
    KEYBOARD_MINUS = GLFW_KEY_MINUS,
    KEYBOARD_PERIOD = GLFW_KEY_PERIOD,
    KEYBOARD_SLASH = GLFW_KEY_SLASH,
    KEYBOARD_0 = GLFW_KEY_0,
    KEYBOARD_1 = GLFW_KEY_1,
    KEYBOARD_2 = GLFW_KEY_2,
    KEYBOARD_3 = GLFW_KEY_3,
    KEYBOARD_4 = GLFW_KEY_4,
    KEYBOARD_5 = GLFW_KEY_5,
    KEYBOARD_6 = GLFW_KEY_6,
    KEYBOARD_7 = GLFW_KEY_7,
    KEYBOARD_8 = GLFW_KEY_8,
    KEYBOARD_9 = GLFW_KEY_9,
    KEYBOARD_SEMICOLON = GLFW_KEY_SEMICOLON,
    KEYBOARD_EQUAL = GLFW_KEY_EQUAL,
    KEYBOARD_A = GLFW_KEY_A,
    KEYBOARD_B = GLFW_KEY_B,
    KEYBOARD_C = GLFW_KEY_C,
    KEYBOARD_D = GLFW_KEY_D,
    KEYBOARD_E = GLFW_KEY_E,
    KEYBOARD_F = GLFW_KEY_F,
    KEYBOARD_G = GLFW_KEY_G,
    KEYBOARD_H = GLFW_KEY_H,
    KEYBOARD_I = GLFW_KEY_I,
    KEYBOARD_J = GLFW_KEY_J,
    KEYBOARD_K = GLFW_KEY_K,
    KEYBOARD_L = GLFW_KEY_L,
    KEYBOARD_M = GLFW_KEY_M,
    KEYBOARD_N = GLFW_KEY_N,
    KEYBOARD_O = GLFW_KEY_O,
    KEYBOARD_P = GLFW_KEY_P,
    KEYBOARD_Q = GLFW_KEY_Q,
    KEYBOARD_R = GLFW_KEY_R,
    KEYBOARD_S = GLFW_KEY_S,
    KEYBOARD_T = GLFW_KEY_T,
    KEYBOARD_U = GLFW_KEY_U,
    KEYBOARD_V = GLFW_KEY_V,
    KEYBOARD_W = GLFW_KEY_W,
    KEYBOARD_X = GLFW_KEY_X,
    KEYBOARD_Y = GLFW_KEY_Y,
    KEYBOARD_Z = GLFW_KEY_Z,
    KEYBOARD_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
    KEYBOARD_BACKSLASH = GLFW_KEY_BACKSLASH,
    KEYBOARD_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
    KEYBOARD_GRAVE = GLFW_KEY_GRAVE_ACCENT,
    KEYBOARD_WORLD_1 = GLFW_KEY_WORLD_1,
    KEYBOARD_WORLD_2 = GLFW_KEY_WORLD_2,
    KEYBOARD_ESCAPE = GLFW_KEY_ESCAPE,
    KEYBOARD_ENTER = GLFW_KEY_ENTER,
    KEYBOARD_TAB = GLFW_KEY_TAB,
    KEYBOARD_BACKSPACE = GLFW_KEY_BACKSPACE,
    KEYBOARD_INSERT = GLFW_KEY_INSERT,
    KEYBOARD_DELETE = GLFW_KEY_DELETE,
    KEYBOARD_RIGHT = GLFW_KEY_RIGHT,
    KEYBOARD_LEFT = GLFW_KEY_LEFT,
    KEYBOARD_DOWN = GLFW_KEY_DOWN,
    KEYBOARD_UP = GLFW_KEY_UP,
    KEYBOARD_PAGE_UP = GLFW_KEY_PAGE_UP,
    KEYBOARD_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
    KEYBOARD_HOME = GLFW_KEY_HOME,
    KEYBOARD_END = GLFW_KEY_END,
    KEYBOARD_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
    KEYBOARD_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
    KEYBOARD_NUM_LOCK = GLFW_KEY_NUM_LOCK,
    KEYBOARD_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
    KEYBOARD_PAUSE = GLFW_KEY_PAUSE,
    KEYBOARD_F1 = GLFW_KEY_F1,
    KEYBOARD_F2 = GLFW_KEY_F2,
    KEYBOARD_F3 = GLFW_KEY_F3,
    KEYBOARD_F4 = GLFW_KEY_F4,
    KEYBOARD_F5 = GLFW_KEY_F5,
    KEYBOARD_F6 = GLFW_KEY_F6,
    KEYBOARD_F7 = GLFW_KEY_F7,
    KEYBOARD_F8 = GLFW_KEY_F8,
    KEYBOARD_F9 = GLFW_KEY_F9,
    KEYBOARD_F10 = GLFW_KEY_F10,
    KEYBOARD_F11 = GLFW_KEY_F11,
    KEYBOARD_F12 = GLFW_KEY_F12,
    KEYBOARD_F13 = GLFW_KEY_F13,
    KEYBOARD_F14 = GLFW_KEY_F14,
    KEYBOARD_F15 = GLFW_KEY_F15,
    KEYBOARD_F16 = GLFW_KEY_F16,
    KEYBOARD_F17 = GLFW_KEY_F17,
    KEYBOARD_F18 = GLFW_KEY_F18,
    KEYBOARD_F19 = GLFW_KEY_F19,
    KEYBOARD_F20 = GLFW_KEY_F20,
    KEYBOARD_F21 = GLFW_KEY_F21,
    KEYBOARD_F22 = GLFW_KEY_F22,
    KEYBOARD_F23 = GLFW_KEY_F23,
    KEYBOARD_F24 = GLFW_KEY_F24,
    KEYBOARD_F25 = GLFW_KEY_F25,
    KEYBOARD_KP_0 = GLFW_KEY_KP_0,
    KEYBOARD_KP_1 = GLFW_KEY_KP_1,
    KEYBOARD_KP_2 = GLFW_KEY_KP_2,
    KEYBOARD_KP_3 = GLFW_KEY_KP_3,
    KEYBOARD_KP_4 = GLFW_KEY_KP_4,
    KEYBOARD_KP_5 = GLFW_KEY_KP_5,
    KEYBOARD_KP_6 = GLFW_KEY_KP_6,
    KEYBOARD_KP_7 = GLFW_KEY_KP_7,
    KEYBOARD_KP_8 = GLFW_KEY_KP_8,
    KEYBOARD_KP_9 = GLFW_KEY_KP_9,
    KEYBOARD_KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
    KEYBOARD_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
    KEYBOARD_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
    KEYBOARD_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
    KEYBOARD_KP_ADD = GLFW_KEY_KP_ADD,
    KEYBOARD_KP_ENTER = GLFW_KEY_KP_ENTER,
    KEYBOARD_KP_EQUAL = GLFW_KEY_KP_EQUAL,
    KEYBOARD_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    KEYBOARD_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
    KEYBOARD_LEFT_ALT = GLFW_KEY_LEFT_ALT,
    KEYBOARD_LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
    KEYBOARD_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
    KEYBOARD_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
    KEYBOARD_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
    KEYBOARD_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
    KEYBOARD_MENU = GLFW_KEY_MENU
};

static const int IDRIVER_KEYBOARDKEYS_ALL[] = {
    KEYBOARD_SPACE,
    KEYBOARD_APOSTROPHE,
    KEYBOARD_COMMA,
    KEYBOARD_MINUS,
    KEYBOARD_PERIOD,
    KEYBOARD_SLASH,
    KEYBOARD_0,
    KEYBOARD_1,
    KEYBOARD_2,
    KEYBOARD_3,
    KEYBOARD_4,
    KEYBOARD_5,
    KEYBOARD_6,
    KEYBOARD_7,
    KEYBOARD_8,
    KEYBOARD_9,
    KEYBOARD_SEMICOLON,
    KEYBOARD_EQUAL,
    KEYBOARD_A,
    KEYBOARD_B,
    KEYBOARD_C,
    KEYBOARD_D,
    KEYBOARD_E,
    KEYBOARD_F,
    KEYBOARD_G,
    KEYBOARD_H,
    KEYBOARD_I,
    KEYBOARD_J,
    KEYBOARD_K,
    KEYBOARD_L,
    KEYBOARD_M,
    KEYBOARD_N,
    KEYBOARD_O,
    KEYBOARD_P,
    KEYBOARD_Q,
    KEYBOARD_R,
    KEYBOARD_S,
    KEYBOARD_T,
    KEYBOARD_U,
    KEYBOARD_V,
    KEYBOARD_W,
    KEYBOARD_X,
    KEYBOARD_Y,
    KEYBOARD_Z,
    KEYBOARD_LEFT_BRACKET,
    KEYBOARD_BACKSLASH,
    KEYBOARD_RIGHT_BRACKET,
    KEYBOARD_GRAVE,
    KEYBOARD_WORLD_1,
    KEYBOARD_WORLD_2,
    KEYBOARD_ESCAPE,
    KEYBOARD_ENTER,
    KEYBOARD_TAB,
    KEYBOARD_BACKSPACE,
    KEYBOARD_INSERT,
    KEYBOARD_DELETE,
    KEYBOARD_RIGHT,
    KEYBOARD_LEFT,
    KEYBOARD_DOWN,
    KEYBOARD_UP,
    KEYBOARD_PAGE_UP,
    KEYBOARD_PAGE_DOWN,
    KEYBOARD_HOME,
    KEYBOARD_END,
    KEYBOARD_CAPS_LOCK,
    KEYBOARD_SCROLL_LOCK,
    KEYBOARD_NUM_LOCK,
    KEYBOARD_PRINT_SCREEN,
    KEYBOARD_PAUSE,
    KEYBOARD_F1,
    KEYBOARD_F2,
    KEYBOARD_F3,
    KEYBOARD_F4,
    KEYBOARD_F5,
    KEYBOARD_F6,
    KEYBOARD_F7,
    KEYBOARD_F8,
    KEYBOARD_F9,
    KEYBOARD_F10,
    KEYBOARD_F11,
    KEYBOARD_F12,
    KEYBOARD_F13,
    KEYBOARD_F14,
    KEYBOARD_F15,
    KEYBOARD_F16,
    KEYBOARD_F17,
    KEYBOARD_F18,
    KEYBOARD_F19,
    KEYBOARD_F20,
    KEYBOARD_F21,
    KEYBOARD_F22,
    KEYBOARD_F23,
    KEYBOARD_F24,
    KEYBOARD_F25,
    KEYBOARD_KP_0,
    KEYBOARD_KP_1,
    KEYBOARD_KP_2,
    KEYBOARD_KP_3,
    KEYBOARD_KP_4,
    KEYBOARD_KP_5,
    KEYBOARD_KP_6,
    KEYBOARD_KP_7,
    KEYBOARD_KP_8,
    KEYBOARD_KP_9,
    KEYBOARD_KP_DECIMAL,
    KEYBOARD_KP_DIVIDE,
    KEYBOARD_KP_MULTIPLY,
    KEYBOARD_KP_SUBTRACT,
    KEYBOARD_KP_ADD,
    KEYBOARD_KP_ENTER,
    KEYBOARD_KP_EQUAL,
    KEYBOARD_LEFT_SHIFT,
    KEYBOARD_LEFT_CONTROL,
    KEYBOARD_LEFT_ALT,
    KEYBOARD_LEFT_SUPER,
    KEYBOARD_RIGHT_SHIFT,
    KEYBOARD_RIGHT_CONTROL,
    KEYBOARD_RIGHT_ALT,
    KEYBOARD_RIGHT_SUPER,
    KEYBOARD_MENU
};

// Keyboard driver.
struct IDriverKeyboard : IDriver
{
    GLFWwindow* window; // Window.
    static IDriverKeyboard* globalKeyboard; // Global keyboard.

    // Create a new keyboard driver.
    IDriverKeyboard(GLFWwindow* window) : window(window) {}

    // Key press action.
    static void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Vfunctions.
    virtual void SupportedButtons(std::vector<int>& supportedButtons) override;
    virtual void RegisterCallbacks() override;

};