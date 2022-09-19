#include "controller.h"

IController::IController(GLFWwindow* window) : keyboard(window), mouse(window)
{

    // Setup the drivers.
    IDriverKeyboard::globalKeyboard = &keyboard;
    drivers[CONTROLLER_DRIVER_KEYBOARD] = &keyboard;
    IDriverMouse::globalMouse = &mouse;
    drivers[CONTROLLER_DRIVER_MOUSE] = &mouse;

    // Init drivers. TODO: REPLACE WITH LOOP!
    drivers[CONTROLLER_DRIVER_KEYBOARD]->Init();
    drivers[CONTROLLER_DRIVER_MOUSE]->Init();

    // Mouse buttons are not configurable.
    mouseButtons.mapping[MOUSE_BT_LEFT].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_LEFT].buttonNum = MOUSE_LEFT_BUTTON;
    mouseButtons.mapping[MOUSE_BT_RIGHT].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_RIGHT].buttonNum = MOUSE_RIGHT_BUTTON;
    mouseButtons.mapping[MOUSE_BT_MIDDLE].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_MIDDLE].buttonNum = MOUSE_MIDDLE_BUTTON;
    mouseButtons.mapping[MOUSE_BT_X_POS].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_X_POS].buttonNum = MOUSE_X_POS;
    mouseButtons.mapping[MOUSE_BT_Y_POS].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_Y_POS].buttonNum = MOUSE_Y_POS;
    mouseButtons.mapping[MOUSE_BT_X_MOVE].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_X_MOVE].buttonNum = MOUSE_X_MOVE;
    mouseButtons.mapping[MOUSE_BT_Y_MOVE].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_Y_MOVE].buttonNum = MOUSE_Y_MOVE;

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
    mouseButtons.Init(this, "res/con/!@#$%^&.conf"); // This won't be able to be opened.
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