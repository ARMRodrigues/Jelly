#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/graphics/material.hpp"
#include "jelly/graphics/shader_interface.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include <memory>

#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

/// @brief A Vulkan-specific implementation of the MaterialInterface.
/// @details This class encapsulates a Vulkan graphics pipeline (`VkPipeline`) and its layout
/// (`VkPipelineLayout`). It defines the complete state of the graphics pipeline for rendering,
/// including shader stages, vertex input, rasterization, and more.
class JELLY_EXPORT VulkanMaterial final : public MaterialInterface {
public:
    /// @brief Constructs a VulkanMaterial with a given shader.
    /// @details The Vulkan pipeline is not created upon construction; it must be explicitly
    /// created by calling `createPipeline()`.
    /// @param shader A shared pointer to the `ShaderInterface` that this material will use.
    explicit VulkanMaterial(std::shared_ptr<jelly::graphics::ShaderInterface> shader);

    /// @brief Creates the Vulkan graphics pipeline for this material.
    /// @details Uses the associated shader and context from the graphics API to build the
    /// `VkPipeline` and `VkPipelineLayout`. This must be called before the material is used.
    /// @param api A pointer to the main `VulkanGraphicAPI` object.
    void createPipeline(VulkanGraphicAPI* api);

    /// @brief Binds the material's Vulkan pipeline to the current command buffer.
    /// @details This method is called during the rendering process to make this material
    /// active for subsequent draw calls.
    void bind() override;

    /// @brief Unbinds the material.
    /// @details In Vulkan, explicitly unbinding a pipeline is often unnecessary, as a new
    /// pipeline is simply bound over the previous one. This method is for API consistency.
    void unbind() override;

private:
    /// @brief The shader program associated with this material.
    std::shared_ptr<jelly::graphics::ShaderInterface> shader_;
    /// @brief The Vulkan pipeline layout, defining shader resources (uniforms, etc.).
    VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
    /// @brief The handle to the compiled Vulkan graphics pipeline object.
    VkPipeline pipeline_ = VK_NULL_HANDLE;

    /// @brief A helper function to create the low-level Vulkan graphics pipeline object.
    /// @param device The logical Vulkan device.
    /// @param renderPass The render pass the pipeline will be compatible with.
    /// @param pipelineLayout The pipeline layout to be used.
    /// @param vertShaderModule The compiled vertex shader module.
    /// @param fragShaderModule The compiled fragment shader module.
    /// @param extent The swapchain extent, used for viewport and scissor configuration.
    /// @return The handle to the newly created `VkPipeline`.
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