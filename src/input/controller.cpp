#include "controller.h"

#include <fstream>
#include <iostream>
#include <sstream>

template<typename T, size_t U>
void IControllerAssignment<T, U>::Init(IController* controller, std::string defaultConfigPath)
{
    this->controller = controller;
    // LoadConfig(defaultConfigPath); TODO: CHECK IF PATH EXISTS!
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

}

template<typename T, size_t U>
void IControllerAssignment<T, U>::SaveConfig(std::string path)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        for (unsigned int i = 0; i < U; i++)
        {
            file << i << ";" << mapping[i].driverNum << ";" << mapping[i].buttonNum << std::endl;
        }
    }
    else std::cout << "ERROR: Failed to save " << path << " to disk!" << std::endl;
}

IController::IController(GLFWwindow* window) : keyboard(window)
{

    // Setup the keyboard.
    IDriverKeyboard::globalKeyboard = &keyboard;
    drivers.push_back(&keyboard);

    // Setup drivers.
    for (auto driver : drivers)
    {
        driver->Init();
    }

    // Backup configs in case saved doesn't exist.
    menuButtons.mapping[MENU_BT_UP].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    menuButtons.mapping[MENU_BT_UP].buttonNum = KEYBOARD_UP;

    // TODO!!!
    return;

    // Button mapping time.
    menuButtons.Init(this, "res/con/MenuButtons.conf");
    cameraButtons.Init(this, "res/con/CameraButtons.conf");
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        playerButtons[i].Init(this, "res/con/PlayerButtons" + std::to_string(i) + ".conf");
    }

}

void IController::Update()
{
    for (auto driver : drivers)
    {
        driver->Update();
    }
}