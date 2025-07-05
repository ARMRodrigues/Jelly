#include "jelly/windowing/glfw/glfw_vulkan_window_system.hpp"

namespace jelly::windowing::glfw {

void GLFWVulkanWindowSystem::createWindow(const WindowSettings &settings) {
    if (window_) {
        throw std::runtime_error("Window already created");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    auto *rawWindow = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);
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
}

}