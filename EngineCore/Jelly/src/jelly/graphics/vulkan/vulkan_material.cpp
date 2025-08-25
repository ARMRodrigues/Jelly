#include "jelly/graphics/vulkan/vulkan_material.hpp"
#include "jelly/graphics/vulkan/vulkan_shader.hpp"
#include "jelly/graphics/graphic_context.hpp"

#include <stdexcept>
#include <iostream>

#include <glm/glm.hpp>

namespace jelly::graphics::vulkan {

struct Vertex {
    float position[3];
    glm::vec2 uv;
};

VulkanMaterial::VulkanMaterial(std::shared_ptr<ShaderInterface> shader)
    : MaterialInterface(shader), shader_(shader)
{}

void VulkanMaterial::updateTexturesDescriptor() {
    auto api = static_cast<VulkanGraphicAPI*>(jelly::graphics::GraphicContext::get().getAPI());
    auto vkShader = static_cast<jelly::graphics::vulkan::VulkanShader*>(shader_.get());
    
    for (uint32_t frame = 0; frame < 2; ++frame) {
        for (auto& [type, texture] : textures_) {
            uint32_t binding = vkShader->getTextureBinding("albedoTexture");
            auto vkTexture = static_cast<VulkanTexture*>(texture.get());
            
            vkShader->updateTextureDescriptor(
                vkTexture->getVkImageView(),
                vkTexture->getVkSampler(),
                binding,
                frame
            );
        }
    }
}

void VulkanMaterial::bind() {
    auto api = static_cast<VulkanGraphicAPI*>(jelly::graphics::GraphicContext::get().getAPI());
    auto vkShader = static_cast<jelly::graphics::vulkan::VulkanShader*>(shader_.get());

    VkCommandBuffer cmd = api->getCurrentCommandBuffer();
    uint32_t frameIndex = api->getCurrentFrameIndex();
    VkDescriptorSet descriptorSet = vkShader->getDescriptorSet(frameIndex);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_.get());
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_.get(), 0, 1, &descriptorSet, 0, nullptr);
}

void VulkanMaterial::setAlbedoTexture(std::shared_ptr<TextureInterface> texture)
{
    textures_[TextureType::Albedo] = std::static_pointer_cast<VulkanTexture>(texture);

    updateTexturesDescriptor();
}

void VulkanMaterial::VulkanMaterial::unbind() {
    // No-op (Vulkan does not require explicit unbind)
}

void VulkanMaterial::release()
{
    if (pipeline_) {
        pipeline_.reset();
        pipelineLayout_.reset();
    }
}

void VulkanMaterial::setVec3(const char* name, const float* vec) {
    auto vulkanShader = std::dynamic_pointer_cast<VulkanShader>(getShader());
    if (vulkanShader)
        vulkanShader->setUniformVec3(name, vec);
}

void VulkanMaterial::setMat4(const char* name, const float* matrix) {
    auto vulkanShader = std::dynamic_pointer_cast<VulkanShader>(getShader());
    if (vulkanShader)
        vulkanShader->setUniformMat4(name, matrix);
}

void VulkanMaterial::createPipeline(VulkanGraphicAPI* api) {
    if (!shader_) {
        std::cerr << "shader_ is nullptr!\n";
        throw std::runtime_error("shader_ is nullptr in VulkanMaterial::createPipeline");
    }

    auto vkShader = std::dynamic_pointer_cast<VulkanShader>(shader_);
    if (!vkShader) {
        throw std::runtime_error("VulkanMaterial requires VulkanShader");
    }

    VkDevice device = api->getDevice();
    VkRenderPass renderPass = api->getRenderPass();
    VkExtent2D extent = api->getSwapchainExtent();

    VkDescriptorSetLayout setLayout = vkShader->getDescriptorSetLayout();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &setLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    VkPipelineLayout rawPipelineLayout = VK_NULL_HANDLE;
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &rawPipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout");
    }

    pipelineLayout_ = jelly::core::ManagedResource<VkPipelineLayout>(
        rawPipelineLayout,
        [device](VkPipelineLayout layout) { vkDestroyPipelineLayout(device, layout, nullptr); },
        VK_NULL_HANDLE
    );

    VkPipeline rawPipeline = createGraphicsPipeline(
        device,
        renderPass,
        pipelineLayout_.get(),
        vkShader->getVertexModule()->getModule(),
        vkShader->getFragmentModule()->getModule(),
        extent);

    pipeline_ = jelly::core::ManagedResource<VkPipeline>(
        rawPipeline,
        [device](VkPipeline pipeline) { vkDestroyPipeline(device, pipeline, nullptr); },
        VK_NULL_HANDLE
    );
}

VkPipeline VulkanMaterial::createGraphicsPipeline(
    VkDevice device,
    VkRenderPass renderPass,
    VkPipelineLayout pipelineLayout,
    VkShaderModule vertShaderModule,
    VkShaderModule fragShaderModule,
    VkExtent2D extent)
{
    VkPipelineShaderStageCreateInfo vertStageInfo{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    vertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertStageInfo.module = vertShaderModule;
    vertStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragStageInfo{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragStageInfo.module = fragShaderModule;
    fragStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertStageInfo, fragStageInfo };

    // Vertex input
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attributeDescriptions[2]{};

    // Position
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    // UV
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, uv);

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    // Viewport e scissor
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = static_cast<float>(extent.height);
    viewport.width = static_cast<float>(extent.width);
    viewport.height = -static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, extent};

    VkPipelineViewportStateCreateInfo viewportState{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Color blend
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = 
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlending{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    // Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;   // <<<<< adicionado
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    VkPipeline pipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create graphics pipeline");
    }

    return pipeline;
}

} // namespace jelly::graphics::vulkan