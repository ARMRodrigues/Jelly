#pragma once

#include "vulkan_shader_module.hpp"
#include "jelly/jelly_export.hpp"
#include "jelly/graphics/shader_interface.hpp"

#include <memory>

namespace jelly::graphics::vulkan {

/// @brief Represents a complete Vulkan shader program consisting of vertex and fragment shader modules.
///
/// This class implements the `ShaderInterface` and holds ownership of both vertex and fragment shaders.
/// It is used as the backend representation of a shader program in Vulkan.
class JELLY_EXPORT VulkanShader : public graphics::ShaderInterface {
public:
    /// @brief Constructs a Vulkan shader program with vertex and fragment shader modules.
    ///
    /// @param vertex The compiled Vulkan vertex shader module.
    /// @param fragment The compiled Vulkan fragment shader module.
    VulkanShader(
        std::unique_ptr<VulkanShaderModule> vertex,
        std::unique_ptr<VulkanShaderModule> fragment)
        : vertex_(std::move(vertex)), fragment_(std::move(fragment)) {
            if (!fragment_) {
                throw std::runtime_error("Fragment shader module is nullptr after VulkanShader construction");
            }
        }

    /// @brief Binds the shader program for rendering.
    ///
    /// No-op for Vulkan; actual binding occurs during pipeline creation.
    void bind() override {}

    /// @brief Unbinds the shader program.
    ///
    /// No-op for Vulkan; unbinding is not explicitly needed.
    void unbind() override {}

    /// @brief Returns the vertex shader module.
    /// @return Pointer to the vertex `VulkanShaderModule`.
    const VulkanShaderModule* getVertexModule() const { 
        if (!vertex_) throw std::runtime_error("Vertex shader module is null");
        return vertex_.get();
    }

    /// @brief Returns the fragment shader module.
    /// @return Pointer to the fragment `VulkanShaderModule`.
    const VulkanShaderModule* getFragmentModule() const {
        if (!fragment_) throw std::runtime_error("Fragment shader module is null");
        return fragment_.get();
    }

private:
    std::unique_ptr<VulkanShaderModule> vertex_;   ///< Vertex shader module.
    std::unique_ptr<VulkanShaderModule> fragment_; ///< Fragment shader module.
};

} // namespace jelly::graphics::vulkan
