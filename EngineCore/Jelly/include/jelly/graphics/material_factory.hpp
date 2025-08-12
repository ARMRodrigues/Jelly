#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/graphics/material.hpp"
#include "jelly/graphics/shader_interface.hpp"
#include "jelly/graphics/graphic_context.hpp"
#include "jelly/core/graphic_api_type.hpp"
#include "jelly/graphics/vulkan/vulkan_material.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include <iostream>

namespace jelly::graphics {

/// @brief A factory for creating API-specific material instances.
///
/// This class provides a static method to abstract the creation of material
/// objects. It determines the current graphics API from the `GraphicContext`
/// and instantiates the appropriate concrete material implementation (e.g., VulkanMaterial).
class JELLY_EXPORT MaterialFactory {
public:
    /// @brief Creates a new material instance configured with a specific shader.
    ///
    /// This static method handles the creation of a material, including the construction
    /// of its internal rendering pipeline, based on the provided shader and the active
    /// graphics API.
    /// @param shader A shared pointer to the shader program that the material will use.
    /// @return A `MaterialHandle` to the newly created, API-specific material.
    /// @throws std::runtime_error if the graphics API reported by the `GraphicContext` is not supported.
    static MaterialHandle create(std::shared_ptr<ShaderInterface> shader) {
        
        switch (GraphicContext::get().getAPIType()) {
            case core::GraphicAPIType::Vulkan: {
                auto material = std::make_shared<vulkan::VulkanMaterial>(shader);
                auto api = static_cast<vulkan::VulkanGraphicAPI*>(GraphicContext::get().getAPI());
                material->createPipeline(api); // The pipeline is created here
                return material;
            }
            default:
                throw std::runtime_error("Unsupported graphics API in MaterialFactory");
        }
    }
};

} // namespace jelly::graphics