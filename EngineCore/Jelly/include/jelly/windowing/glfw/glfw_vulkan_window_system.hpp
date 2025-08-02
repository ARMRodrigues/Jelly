#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "glfw_window_system.hpp"

#include "jelly/windowing/vulkan_native_window_handle_provider.hpp"

namespace jelly::windowing::glfw {

using jelly::windowing::VulkanNativeWindowHandleProvider;
 
class GLFWVulkanWindowSystem : 
    public GLFWWindowSystem, 
    public VulkanNativeWindowHandleProvider {
public:
    void createWindow(const WindowSettings& settings) override;
    void getFramebufferSize(uint32_t& width, uint32_t& height) override;
    void waitEvents() override;
    
    void* getNativeWindowHandle() override;
    
    std::vector<const char*> getVulkanRequiredExtensions();
    VkSurfaceKHR createVulkanSurface(VkInstance instance);
};

}