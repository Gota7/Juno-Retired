#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Settings.
#define SCR_WIDTH 800
#define SCR_HEIGHT 600

typedef void (*WindowCallback)(GLFWwindow* window);
typedef void (*FramebufferSizeCallback)(GLFWwindow* window, int width, int height);

// Init window.
GLFWwindow* Window_Init(FramebufferSizeCallback sizeCallback);

// Main window.
void Window_Main(GLFWwindow* window, WindowCallback callback = nullptr);

// Close window.
void Window_Close();