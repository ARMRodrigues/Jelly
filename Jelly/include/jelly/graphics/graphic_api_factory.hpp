#pragma once

#include <memory>

#include "jelly/core/graphic_api_type.hpp"
#include "jelly/graphics/graphic_api_interface.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

namespace jelly::graphics {

using jelly::core::GraphicAPIType;

/// Factory responsible for creating GraphicAPIInterface instances based on enum type.
class GraphicsAPIFactory {
public:
    /// Creates an GraphicAPIInterface instance based on the given API type.
    static std::unique_ptr<GraphicAPIInterface> create(GraphicAPIType graphicAPIType) {
        switch (graphicAPIType) {
            case GraphicAPIType::Vulkan:
                return std::make_unique<vulkan::VulkanGraphicAPI>();
            default:
                return nullptr;
        }
    }
};

}
