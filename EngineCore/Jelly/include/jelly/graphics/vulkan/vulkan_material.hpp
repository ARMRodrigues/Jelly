#pragma once

#include "vulkan_graphic_api.hpp"

#include "jelly/jelly_export.hpp"
#include "jelly/core/managed_resource.hpp"
#include "jelly/graphics/material.hpp"
#include "jelly/graphics/shader_interface.hpp"

#include <vulkan/vulkan.h>

#include <memory>


namespace jelly::graphics::vulkan {

/// @brief Vulkan implementation of MaterialInterface
class JELLY_EXPORT VulkanMaterial final : public MaterialInterface {
public:
    /// @brief Creates material with specified shader
    explicit VulkanMaterial(std::shared_ptr<jelly::graphics::ShaderInterface> shader);

    /// @brief Initializes Vulkan pipeline for this material
    void createPipeline(VulkanGraphicAPI* api);

    /// @brief Binds the material's pipeline for rendering
    void bind() override;
    
    /// @brief Unbinds the material (Vulkan typically doesn't require this)
    void unbind() override;

    /// @brief Sets vec3 uniform value
    /// @param name Name of uniform in shader
    /// @param vec Pointer to 3 consecutive floats
    void setVec3(const char* name, const float* vec) override;

    /// @brief Sets mat4 uniform value
    /// @param name Name of uniform in shader
    /// @param matrix Pointer to 16 consecutive floats (column-major)
    void setMat4(const char* name, const float* matrix) override;

private:
    std::shared_ptr<jelly::graphics::ShaderInterface> shader_;

    // Auto-managed Vulkan resources
    jelly::core::ManagedResource<VkPipeline> pipeline_;
    jelly::core::ManagedResource<VkPipelineLayout> pipelineLayout_;

    /// @brief Creates Vulkan graphics pipeline
    VkPipeline createGraphicsPipeline(
        VkDevice device,
        VkRenderPass renderPass,
        VkPipelineLayout pipelineLayout,
        VkShaderModule vertShaderModule,
        VkShaderModule fragShaderModule,
        VkExtent2D extent
    );
};

} // namespace jelly::graphics::vulkan