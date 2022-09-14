#include "controller.h"

#include <fstream>
#include <iostream>
#include <sstream>

template<typename T>
void IControllerAssignment<T>::Init(IController* controller, std::string defaultConfigPath)
{
    this->controller = controller;
    LoadConfig(defaultConfigPath);
}

template<typename T>
float IControllerAssignment<T>::ButtonDown(T button)
{
    IControllerAssignmentInfo& info = mapping[button];
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum];
}

template<typename T>
bool IControllerAssignment<T>::ButtonUp(T button)
{
    IControllerAssignmentInfo& info = mapping[button];
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum] == 0.0f;
}

template<typename T>
float IControllerAssignment<T>::ButtonPressed(T button)
{
    IControllerAssignmentInfo& info = mapping[button];
    if (controller->drivers[info.driverNum]->prevInputs[info.buttonNum] != 0.0f) return 0.0f; // Was down previous frame.
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum];
}

template<typename T>
bool IControllerAssignment<T>::ButtonReleased(T button)
{
    IControllerAssignmentInfo& info = mapping[button];
    if (controller->drivers[info.driverNum]->prevInputs[info.buttonNum] == 0.0f) return false; // Was released previous frame.
    return controller->drivers[info.driverNum]->currInputs[info.buttonNum] == 0.0f;
}

template<typename T>
void IControllerAssignment<T>::LoadConfig(std::string path)
{

}

template<typename T>
void IControllerAssignment<T>::SaveConfig(std::string path)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        for (auto const& [key, val] : mapping)
        {
            file << key << ";" << val.driverNum << ";" << val.buttonNum << std::endl;
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