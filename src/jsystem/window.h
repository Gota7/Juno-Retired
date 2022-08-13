#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

// Settings.
#define SCR_WIDTH 1024
#define SCR_HEIGHT 576
//#define DEBUG

typedef void (*WindowCallback)(GLFWwindow* window);

// Init window.
GLFWwindow* Window_Init(std::string title);

// Main window.
void Window_Main(GLFWwindow* window, WindowCallback callback = nullptr);

// Close window.
void Window_Close();