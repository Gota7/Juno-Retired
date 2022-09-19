#include "controller.h"

IController::IController(GLFWwindow* window) : keyboard(window)
{

    // Setup the keyboard.
    IDriverKeyboard::globalKeyboard = &keyboard;
    drivers[CONTROLLER_DRIVER_KEYBOARD] = &keyboard;

    // Init drivers. TODO: REPLACE WITH LOOP!
    drivers[CONTROLLER_DRIVER_KEYBOARD]->Init();

    // Backup configs in case saved doesn't exist.
    menuButtons.mapping[MENU_BT_UP].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    menuButtons.mapping[MENU_BT_UP].buttonNum = KEYBOARD_UP;
    menuButtons.mapping[MENU_BT_DOWN].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    menuButtons.mapping[MENU_BT_DOWN].buttonNum = KEYBOARD_DOWN;
    menuButtons.mapping[MENU_BT_LEFT].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    menuButtons.mapping[MENU_BT_LEFT].buttonNum = KEYBOARD_LEFT;
    menuButtons.mapping[MENU_BT_RIGHT].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    menuButtons.mapping[MENU_BT_RIGHT].buttonNum = KEYBOARD_RIGHT;
    menuButtons.mapping[MENU_BT_SELECT].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    menuButtons.mapping[MENU_BT_SELECT].buttonNum = KEYBOARD_Z;
    menuButtons.mapping[MENU_BT_BACK].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    menuButtons.mapping[MENU_BT_BACK].buttonNum = KEYBOARD_X;

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