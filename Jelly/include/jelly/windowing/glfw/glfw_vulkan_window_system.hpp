#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "glfw_window_system.hpp"

namespace jelly::windowing::glfw {

class GLFWVulkanWindowSystem : public GLFWWindowSystem {
public:
    void createWindow(const WindowSettings& settings) override;
};

}