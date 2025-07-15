#include "jelly/windowing/glfw/glfw_vulkan_window_system.hpp"

#include "jelly/exception.hpp"

#include <iostream>

namespace jelly::windowing::glfw {

void GLFWVulkanWindowSystem::createWindow(const WindowSettings &settings) {
    if (window_) {
        throw jelly::Exception("Window already created");
    }

    if (!glfwInit()) {
        throw jelly::Exception("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    auto *rawWindow = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);
    
    // TODO : remove, only to tast bindings in C#
    glfwMakeContextCurrent(rawWindow);
    glfwSwapInterval(settings.vsync ? 1 : 0);
    //

    if (!rawWindow)
    {
        throw jelly::Exception("Failed to create Vulkan-compatible GLFW window");
    }

    window_ = ManagedResource<GLFWwindow *>(
        rawWindow,
        [](GLFWwindow *w)
        {
            if (w) {
                glfwDestroyWindow(w);
            }
        },
        nullptr
    );

    glfwShowWindow(window_.get());
    glfwRestoreWindow(window_.get());
    glfwFocusWindow(window_.get());
}

void GLFWVulkanWindowSystem::getFramebufferSize(uint32_t &width, uint32_t &height)
{
    int iw, ih;
    glfwGetFramebufferSize(window_.get(), &iw, &ih);
    width = static_cast<uint32_t>(iw);
    height = static_cast<uint32_t>(ih);
}

void GLFWVulkanWindowSystem::waitEvents()
{
    glfwWaitEvents();
}

void *GLFWVulkanWindowSystem::getNativeWindowHandle()
{
    return window_.get();
}

std::vector<const char *> GLFWVulkanWindowSystem::getVulkanRequiredExtensions()
{
    uint32_t count = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);
    return std::vector<const char*>{extensions, extensions + count};
}

VkSurfaceKHR GLFWVulkanWindowSystem::createVulkanSurface(VkInstance instance)
{
    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, window_.get(), nullptr, &surface) != VK_SUCCESS)
    {
        throw jelly::Exception("Failed to create Vulkan surface!");
    }

    return surface;
}

}