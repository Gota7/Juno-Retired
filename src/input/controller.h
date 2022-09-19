#pragma once

#include "buttons.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"

#include <array>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

#define NUM_PLAYERS 4

// Controller drivers.
enum IControllerDrivers
{
    CONTROLLER_DRIVER_KEYBOARD,
    CONTROLLER_DRIVER_MOUSE,
    CONTROLLER_DRIVER_GAMEPAD_1,
    CONTROLLER_DRIVER_GAMEPAD_2,
    CONTROLLER_DRIVER_GAMEPAD_3,
    CONTROLLER_DRIVER_GAMEPAD_4,

    CONTROLLER_DRIVER_COUNT
};

// Controller assignment info.
struct IControllerAssignmentInfo
{
    int driverNum; // Driver number.
    int buttonNum; // Button of the controller.
};

// A controller assignment.
struct IController;
template<typename T, size_t U>
struct IControllerAssignment
{
    IController* controller;
    IControllerAssignmentInfo mapping[U];

    // Initialize.
    void Init(IController* controller, std::string defaultConfigPath);

    // Button down. Has 0-1 floating range.
    float ButtonDown(T button);

    // Button up.
    bool ButtonUp(T button);

    // Button pressed. Has 0-1 floating range.
    float ButtonPressed(T button);

    // Button released.
    bool ButtonReleased(T button);

    // Load a config.
    void LoadConfig(std::string path);

    // Save a config.
    void SaveConfig(std::string path);

};

// Main controls for the game.
struct IController
{
    IDriverKeyboard keyboard;
    IDriverMouse mouse;
    IDriver* drivers[CONTROLLER_DRIVER_COUNT];
    IControllerAssignment<IMouseButtons, MOUSE_BT_COUNT> mouseButtons;
    IControllerAssignment<IMenuButtons, MENU_BT_COUNT> menuButtons;
    IControllerAssignment<ICameraButtons, CAMERA_BT_COUNT> cameraButtons;
    IControllerAssignment<IPlayerButtons, PLAYER_BT_COUNT> playerButtons[NUM_PLAYERS];

    // Make a new controller.
    IController(GLFWwindow* window);

    // Update the controller.
    void Update();

};

template<typename T, size_t U>
void IControllerAssignment<T, U>::Init(IController* controller, std::string defaultConfigPath)
{
    this->controller = controller;
    std::ifstream file;
    file.open(defaultConfigPath);
    if (file)
    {
        file.close();
        LoadConfig(defaultConfigPath);
    }
}

template<typename T, size_t U>
float IControllerAssignment<T, U>::ButtonDown(T button)
{
    IControllerAssignmentInfo& info = mapping[(int)button];
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum];
}

template<typename T, size_t U>
bool IControllerAssignment<T, U>::ButtonUp(T button)
{
    IControllerAssignmentInfo& info = mapping[(int)button];
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum] == 0.0f;
}

template<typename T, size_t U>
float IControllerAssignment<T, U>::ButtonPressed(T button)
{
    IControllerAssignmentInfo& info = mapping[(int)button];
    if (controller->drivers[info.driverNum]->prevInputs[info.buttonNum] != 0.0f) return 0.0f; // Was down previous frame.
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum];
}

template<typename T, size_t U>
bool IControllerAssignment<T, U>::ButtonReleased(T button)
{
    IControllerAssignmentInfo& info = mapping[(int)button];
    if (controller->drivers[info.driverNum]->prevInputs[info.buttonNum] == 0.0f) return false; // Was released previous frame.
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum] == 0.0f;
}

template<typename T, size_t U>
void IControllerAssignment<T, U>::LoadConfig(std::string path)
{
    // TODO!!!
}

template<typename T, size_t U>
void IControllerAssignment<T, U>::SaveConfig(std::string path)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        for (unsigned int i = 0; i < U; i++)
        {
            file << mapping[i].driverNum << ";" << mapping[i].buttonNum << std::endl;
        }
    }
    else std::cout << "ERROR: Failed to save " << path << " to disk!" << std::endl;
}