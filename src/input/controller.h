#pragma once

#include "buttons.h"
#include "drivers/keyboard.h"

#include <GLFW/glfw3.h>
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
    CONTROLLER_DRIVER_GAMEPAD_4
};

// Controller assignment info.
struct IControllerAssignmentInfo
{
    int driverNum; // Driver number.
    int buttonNum; // Button of the controller.
};

// A controller assignment.
struct IController;
template<typename T>
struct IControllerAssignment
{
    IController* controller;
    std::map<T, IControllerAssignmentInfo> mapping;
    
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
    std::vector<IDriver*> drivers;
    IControllerAssignment<IMenuButtons> menuButtons;
    IControllerAssignment<ICameraButtons> cameraButtons;
    IControllerAssignment<IPlayerButtons> playerButtons[NUM_PLAYERS];

    // Make a new controller.
    IController(GLFWwindow* window);

    // Update the controller.
    void Update();

};