#include "repeater.h"

#include "../jsystem.h"

IController ctrl;

IRepeater::IRepeater()
{
    lastTime = (float)JFrame::currentFrame;
}

float IRepeater::Poll()
{
    auto h = ctrl.menuButtons.ButtonDown;
}