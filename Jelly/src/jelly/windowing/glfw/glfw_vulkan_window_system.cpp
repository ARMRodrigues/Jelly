#include "jelly/windowing/glfw/glfw_vulkan_window_system.hpp"

#include <iostream>

namespace jelly::windowing::glfw {

void GLFWVulkanWindowSystem::createWindow(const WindowSettings &settings) {
    if (window_) {
        throw std::runtime_error("Window already created");
    }

    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    auto *rawWindow = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);
    
    // TODO : remove, only to tast bindings in C#
    glfwMakeContextCurrent(rawWindow);
    glfwSwapInterval(settings.vsync ? 1 : 0);
    //

    if (!rawWindow)
    {
        throw std::runtime_error("Failed to create Vulkan-compatible GLFW window");
    }

    window_ = ManagedResource<GLFWwindow *>(
        rawWindow,
        [](GLFWwindow *w)
        {
            if (w)
                glfwDestroyWindow(w);
        },
        nullptr
    );

    glfwShowWindow(window_.get());
    glfwRestoreWindow(window_.get());
    glfwFocusWindow(window_.get());
}

}