#pragma once

#include "constants.h"
#include <glad/glad.h>
#ifdef VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#include <string>

#ifdef VULKAN
#include <vulkan/vulkan.h>
#endif

// Vulkan instance variables.
#ifdef VULKAN
extern VkInstance VulkanInstance;
#endif

typedef void (*WindowCallback)(GLFWwindow* window);

// Init window.
GLFWwindow* Window_Init(std::string title);

// Main window.
void Window_Main(GLFWwindow* window, WindowCallback callback = nullptr);

// Close window.
void Window_Close(GLFWwindow* window);