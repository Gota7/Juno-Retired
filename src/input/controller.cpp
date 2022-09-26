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
    mouseButtons.mapping[MOUSE_BT_X_MOVE_L].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_Y_MOVE_L].buttonNum = MOUSE_X_MOVE_L;
    mouseButtons.mapping[MOUSE_BT_X_MOVE_R].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_Y_MOVE_R].buttonNum = MOUSE_X_MOVE_R;
    mouseButtons.mapping[MOUSE_BT_X_MOVE_M].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_Y_MOVE_M].buttonNum = MOUSE_X_MOVE_M;
    mouseButtons.mapping[MOUSE_BT_SCROLL_X_POS].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_SCROLL_X_POS].buttonNum = MOUSE_SCROLL_X_POS;
    mouseButtons.mapping[MOUSE_BT_SCROLL_Y_POS].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_SCROLL_Y_POS].buttonNum = MOUSE_SCROLL_Y_POS;
    mouseButtons.mapping[MOUSE_BT_SCROLL_X_MOVE].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_SCROLL_X_MOVE].buttonNum = MOUSE_SCROLL_X_MOVE;
    mouseButtons.mapping[MOUSE_BT_SCROLL_Y_MOVE].driverNum = CONTROLLER_DRIVER_MOUSE;
    mouseButtons.mapping[MOUSE_BT_SCROLL_Y_MOVE].buttonNum = MOUSE_SCROLL_Y_MOVE;

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

    // Free cam buttons.
    freeCamButtons.mapping[FREECAM_BT_FORWARD].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    freeCamButtons.mapping[FREECAM_BT_FORWARD].buttonNum = KEYBOARD_W;
    freeCamButtons.mapping[FREECAM_BT_BACKWARD].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    freeCamButtons.mapping[FREECAM_BT_BACKWARD].buttonNum = KEYBOARD_S;
    freeCamButtons.mapping[FREECAM_BT_FORWARD_BACKWARD_AN].driverNum = CONTROLLER_DRIVER_MOUSE;
    freeCamButtons.mapping[FREECAM_BT_FORWARD_BACKWARD_AN].buttonNum = MOUSE_SCROLL_Y_MOVE;
    freeCamButtons.mapping[FREECAM_BT_LEFT].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    freeCamButtons.mapping[FREECAM_BT_LEFT].buttonNum = KEYBOARD_A;
    freeCamButtons.mapping[FREECAM_BT_RIGHT].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    freeCamButtons.mapping[FREECAM_BT_RIGHT].buttonNum = KEYBOARD_D;
    freeCamButtons.mapping[FREECAM_BT_LEFT_RIGHT_AN].driverNum = CONTROLLER_DRIVER_MOUSE;
    freeCamButtons.mapping[FREECAM_BT_LEFT_RIGHT_AN].buttonNum = MOUSE_X_MOVE_L;
    freeCamButtons.mapping[FREECAM_BT_UP].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    freeCamButtons.mapping[FREECAM_BT_UP].buttonNum = KEYBOARD_SPACE;
    freeCamButtons.mapping[FREECAM_BT_DOWN].driverNum = CONTROLLER_DRIVER_KEYBOARD;
    freeCamButtons.mapping[FREECAM_BT_DOWN].buttonNum = KEYBOARD_LEFT_SHIFT;
    freeCamButtons.mapping[FREECAM_BT_UP_DOWN_AN].driverNum = CONTROLLER_DRIVER_MOUSE;
    freeCamButtons.mapping[FREECAM_BT_UP_DOWN_AN].buttonNum = MOUSE_Y_MOVE_L;
    freeCamButtons.mapping[FREECAM_BT_ROT_X_AN].driverNum = CONTROLLER_DRIVER_MOUSE;
    freeCamButtons.mapping[FREECAM_BT_ROT_X_AN].buttonNum = MOUSE_X_MOVE_R;
    freeCamButtons.mapping[FREECAM_BT_ROT_Y_AN].driverNum = CONTROLLER_DRIVER_MOUSE;
    freeCamButtons.mapping[FREECAM_BT_ROT_Y_AN].buttonNum = MOUSE_Y_MOVE_R;

    // Button mapping time.
    mouseButtons.Init(this, "res/con/!@#$%^&.conf"); // This won't be able to be opened.
    menuButtons.Init(this, "res/con/MenuButtons.conf");
    freeCamButtons.Init(this, "res/con/FreeCamButtons.conf");
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
        //driver->Update(); TODO: FINISH THINGS FIRST!!!
    }
    drivers[CONTROLLER_DRIVER_KEYBOARD]->Update();
    drivers[CONTROLLER_DRIVER_MOUSE]->Update();
}