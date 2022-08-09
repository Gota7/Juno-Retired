#include "frame.h"

double JFrame::currentFrame = 0;
double JFrame::lastFrame = 0;
float JFrame::deltaTime = 0;

void JFrame::Update()
{
    lastFrame = currentFrame;
    currentFrame = glfwGetTime();
    deltaTime = static_cast<float>(currentFrame - lastFrame);
}