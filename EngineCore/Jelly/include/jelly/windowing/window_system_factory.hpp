#pragma once

#include "jelly/core/graphic_api_type.hpp"
#include "jelly/windowing/window_system_interface.hpp"
#include "jelly/windowing/glfw/glfw_vulkan_window_system.hpp"

#include <memory>

namespace jelly::windowing {

using jelly::core::GraphicAPIType;

/// Factory responsible for creating WindowSystemInterface instances based on graphics API.
class WindowSystemFactory {
public:
    /// Creates a WindowSystemInterface based on the given graphics API.
    /// <param name="graphicsAPIType">The graphics API to use (e.g., Vulkan).</param>
    /// <returns>A unique_ptr to the appropriate WindowSystemInterface implementation.</returns>
    static std::unique_ptr<WindowSystemInterface> create(GraphicAPIType graphicAPIType) {
        switch (graphicAPIType) {
            case GraphicAPIType::Vulkan:
                return std::make_unique<glfw::GLFWVulkanWindowSystem>();
            default:
                return nullptr;
        }
    }
};

}
