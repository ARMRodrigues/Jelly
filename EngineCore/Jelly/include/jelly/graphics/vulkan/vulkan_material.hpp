#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/graphics/material.hpp"
#include "jelly/graphics/shader_interface.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"
#include "jelly/core/managed_resource.hpp"

#include <memory>
#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

class JELLY_EXPORT VulkanMaterial final : public MaterialInterface {
public:
    explicit VulkanMaterial(std::shared_ptr<jelly::graphics::ShaderInterface> shader);

    void createPipeline(VulkanGraphicAPI* api);

    void bind() override;
    void unbind() override;

private:
    std::shared_ptr<jelly::graphics::ShaderInterface> shader_;

    // Use ManagedResource to automatically cleanup pipeline and layout
    jelly::core::ManagedResource<VkPipeline> pipeline_;
    jelly::core::ManagedResource<VkPipelineLayout> pipelineLayout_;

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
