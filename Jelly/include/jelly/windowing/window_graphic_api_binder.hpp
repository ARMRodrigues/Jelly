#pragma once

#include "window_system_interface.hpp"
#include "vulkan_native_window_handle_provider.hpp"

#include "jelly/core/graphic_api_type.hpp"
#include "jelly/graphics/graphic_api_interface.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

namespace jelly::windowing {

class WindowGraphicAPIBinder {
public:
    static bool bind(
        jelly::core::GraphicAPIType type,
        WindowSystemInterface* window,
        jelly::graphics::GraphicAPIInterface* graphics) {
        
        switch (type) {
            case core::GraphicAPIType::Vulkan: {
                auto* vkWindow = dynamic_cast<VulkanNativeWindowHandleProvider*>(window);
                auto* vkGraphics = dynamic_cast<jelly::graphics::vulkan::VulkanGraphicAPI*>(graphics);
                if (!vkWindow || !vkGraphics)
                    return false;

                vkGraphics->setWindowProvider(vkWindow);
                return true;
            }
            default:
                return false;
        }
        }
};

}
