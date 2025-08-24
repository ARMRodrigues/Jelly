#include "jelly/graphics/vulkan/vulkan_shader.hpp"

#include <stdexcept>
#include <string.h>

#include <iostream>

namespace jelly::graphics::vulkan {

VulkanShader::VulkanShader(
    VulkanGraphicAPI* api,
    std::unique_ptr<VulkanShaderModule> vertex,
    std::unique_ptr<VulkanShaderModule> fragment)
    : api_(api), vertex_(std::move(vertex)), fragment_(std::move(fragment))
{
    if (!fragment_) {
        throw std::runtime_error("Fragment shader module is nullptr after VulkanShader construction");
    }

    reflectUniforms();
    createUniformBuffers();
    createDescriptorSetLayout();
    createDescriptorPool();
    allocateDescriptorSets();
    initializeDefaultTexture();
    updateDescriptorSets();
}

void VulkanShader::reflectUniforms() {
    reflectShaderCode(vertex_->getSPIRVCode());
    reflectShaderCode(fragment_->getSPIRVCode());
}

void VulkanShader::reflectShaderCode(const std::vector<uint8_t>& code) {
    SpvReflectShaderModule module;
    spvReflectCreateShaderModule(code.size(), code.data(), &module); // já em bytes

    uint32_t count = 0;
    spvReflectEnumerateDescriptorBindings(&module, &count, nullptr);

    std::vector<SpvReflectDescriptorBinding*> bindings(count);
    spvReflectEnumerateDescriptorBindings(&module, &count, bindings.data());

    for (auto* b : bindings) {
        if (b->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
            uniformBufferSize = b->block.size; 
            cpuUniformData.resize(uniformBufferSize); 
            
            for (uint32_t i = 0; i < b->block.member_count; i++) {
                auto& member = b->block.members[i]; 
                uniformOffsets[member.name] = member.offset;
            }
        }
        else if (b->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
            TextureBinding tb{};
            tb.binding = b->binding;
            tb.set     = b->set;
            tb.name    = b->name ? b->name : "";

            boundTextures[b->binding] = tb;
            textureNameToBinding[tb.name] = b->binding;
        }
    }

    spvReflectDestroyShaderModule(&module);
}

void VulkanShader::createUniformBuffers() {
    if (uniformBufferSize == 0) {
        throw std::runtime_error("Uniform buffer size is zero — shader has no UBO?");
    }

    VkDevice device = api_->getDevice();
    VkPhysicalDevice physicalDevice = api_->getPhysicalDevice();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = uniformBufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBuffer buffer;
        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create uniform buffer");
        }

        uniformBuffers_[i] = jelly::core::ManagedResource<VkBuffer>(
            buffer, [device](VkBuffer b){ vkDestroyBuffer(device, b, nullptr); }, VK_NULL_HANDLE
        );

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        uint32_t memoryTypeIndex = UINT32_MAX;
        for (uint32_t j = 0; j < memProperties.memoryTypeCount; j++) {
            if ((memRequirements.memoryTypeBits & (1 << j)) &&
                (memProperties.memoryTypes[j].propertyFlags &
                 (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) ==
                 (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
                memoryTypeIndex = j;
                break;
            }
        }

        if (memoryTypeIndex == UINT32_MAX) {
            throw std::runtime_error("Failed to find suitable memory type for uniform buffer");
        }

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = memoryTypeIndex;

        VkDeviceMemory memory;
        if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate uniform buffer memory");
        }

        uniformBufferMemories_[i] = jelly::core::ManagedResource<VkDeviceMemory>(
            memory, [device](VkDeviceMemory m){ vkFreeMemory(device, m, nullptr); }, VK_NULL_HANDLE
        );

        vkBindBufferMemory(device, buffer, memory, 0);
    }
}

void VulkanShader::createDescriptorSetLayout() {
    VkDevice device = api_->getDevice();

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        uboLayoutBinding,
        samplerLayoutBinding
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    VkDescriptorSetLayout layout;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor set layout");
    }

    descriptorSetLayout_ = jelly::core::ManagedResource<VkDescriptorSetLayout>(
        layout,
        [device](VkDescriptorSetLayout l) { vkDestroyDescriptorSetLayout(device, l, nullptr); },
        VK_NULL_HANDLE
    );
}

void VulkanShader::createDescriptorPool() {
    VkDevice device = api_->getDevice();
    
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = MAX_FRAMES_IN_FLIGHT;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPool pool;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    descriptorPool_ = jelly::core::ManagedResource<VkDescriptorPool>(
        pool,
        [device](VkDescriptorPool p) { vkDestroyDescriptorPool(device, p, nullptr); },
        VK_NULL_HANDLE
    );
}

void VulkanShader::allocateDescriptorSets() {
    VkDevice device = api_->getDevice();
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout_.get());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool_.get();
    allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets_.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets");
    }
}

void VulkanShader::initializeDefaultTexture() {
    VkDevice device = api_->getDevice();
    VkPhysicalDevice physicalDevice = api_->getPhysicalDevice();

    std::vector<uint8_t> whitePixel = {255, 255, 255, 255};

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageInfo.extent.width = 1;
    imageInfo.extent.height = 1;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkImage image;
    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create default texture image");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    uint32_t memoryTypeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        if ((memRequirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
            memoryTypeIndex = i;
            break;
        }
    }

    if (memoryTypeIndex == UINT32_MAX) {
        throw std::runtime_error("Failed to find memory type for default texture");
    }

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    VkDeviceMemory imageMemory;
    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate memory for default texture");
    }

    vkBindImageMemory(device, image, imageMemory, 0);

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image view for default texture");
    }

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    VkSampler sampler;
    if (vkCreateSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create sampler for default texture");
    }

    for (auto& [binding, tex] : boundTextures) {
        tex.imageView = imageView;
        tex.sampler = sampler;
    }

    defaultTextureImage_ = jelly::core::ManagedResource<VkImage>(image, [device](VkImage i){ vkDestroyImage(device, i, nullptr); }, VK_NULL_HANDLE);
    defaultTextureMemory_ = jelly::core::ManagedResource<VkDeviceMemory>(imageMemory, [device](VkDeviceMemory m){ vkFreeMemory(device, m, nullptr); }, VK_NULL_HANDLE);
    defaultTextureView_ = jelly::core::ManagedResource<VkImageView>(imageView, [device](VkImageView v){ vkDestroyImageView(device, v, nullptr); }, VK_NULL_HANDLE);
    defaultTextureSampler_ = jelly::core::ManagedResource<VkSampler>(sampler, [device](VkSampler s){ vkDestroySampler(device, s, nullptr); }, VK_NULL_HANDLE);
}

void VulkanShader::updateDescriptorSets() {
    VkDevice device = api_->getDevice();

    std::vector<const VulkanShaderModule*> modules = { vertex_.get(), fragment_.get() };
    std::unordered_map<uint32_t, VkDescriptorType> bindingTypes;

    for (const auto* module : modules) {
        SpvReflectShaderModule reflectModule;
        const auto& code = module->getSPIRVCode();
        spvReflectCreateShaderModule(
            code.size(),
            code.data(),
            &reflectModule
        );

        uint32_t count = 0;
        spvReflectEnumerateDescriptorBindings(&reflectModule, &count, nullptr);
        std::vector<SpvReflectDescriptorBinding*> bindings(count);
        spvReflectEnumerateDescriptorBindings(&reflectModule, &count, bindings.data());

        for (auto* binding : bindings) {
            bindingTypes[binding->binding] = static_cast<VkDescriptorType>(binding->descriptor_type);
        }

        spvReflectDestroyShaderModule(&reflectModule);
    }

    for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; ++frame) {
        std::vector<VkWriteDescriptorSet> writes;

        for (auto& [binding, type] : bindingTypes) {
            VkWriteDescriptorSet write{};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = descriptorSets_[frame];
            write.dstBinding = binding;
            write.dstArrayElement = 0;
            write.descriptorCount = 1;
            write.descriptorType = type;

            if (type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers_[frame].get();
                bufferInfo.offset = 0;
                bufferInfo.range = uniformBufferSize;

                write.pBufferInfo = &bufferInfo;
                writes.push_back(write);
            } else if (type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = boundTextures[binding].imageView ? boundTextures[binding].imageView : defaultTextureView_.get();
                imageInfo.sampler   = boundTextures[binding].sampler   ? boundTextures[binding].sampler   : defaultTextureSampler_.get();

                write.pImageInfo = &imageInfo;
                writes.push_back(write);
            }
        }

        if (!writes.empty()) {
            vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
        }
    }
}

void VulkanShader::flushUniforms(uint32_t frameIndex) {
    void* data;
    vkMapMemory(api_->getDevice(), uniformBufferMemories_[frameIndex], 0, uniformBufferSize, 0, &data);
    memcpy(data, cpuUniformData.data(), uniformBufferSize);
    vkUnmapMemory(api_->getDevice(), uniformBufferMemories_[frameIndex]);
}

void VulkanShader::bind() {
    // Vulkan does not need explicit bind here
}

void VulkanShader::unbind() {
    // Vulkan does not need explicit unbind
}

void VulkanShader::release() {
    if (api_ && api_->getDevice() != VK_NULL_HANDLE) {
        fragment_.reset();
        vertex_.reset();

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            uniformBuffers_[i].reset();
            uniformBufferMemories_[i].reset();
        }

        descriptorSetLayout_.reset();
        descriptorPool_.reset();

        descriptorSets_.fill(VK_NULL_HANDLE);

        defaultTextureSampler_.reset();
        defaultTextureView_.reset();
        defaultTextureMemory_.reset();
        defaultTextureImage_.reset();
    }
}

void VulkanShader::setUniformVec3(const char* name, const float* vec) {
    auto it = uniformOffsets.find(name);
    if (it == uniformOffsets.end()) return;
    memcpy(cpuUniformData.data() + it->second, vec, sizeof(float) * 3);
    flushUniforms(api_->getCurrentFrameIndex());
}

void VulkanShader::setUniformMat4(const char* name, const float* matrix) {
    auto it = uniformOffsets.find(name);
    if (it == uniformOffsets.end()) return;
    memcpy(cpuUniformData.data() + it->second, matrix, sizeof(float) * 16);
    flushUniforms(api_->getCurrentFrameIndex());
}

void VulkanShader::updateTextureDescriptor(VkImageView imageView, VkSampler sampler, uint32_t binding, uint32_t frameIndex)
{
    VkDevice device = api_->getDevice();
    
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSets_[frameIndex];
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}

const VulkanShaderModule* VulkanShader::getVertexModule() const {
    if (!vertex_) throw std::runtime_error("Vertex shader module is null");
    return vertex_.get();
}

const VulkanShaderModule* VulkanShader::getFragmentModule() const {
    if (!fragment_) throw std::runtime_error("Fragment shader module is null");
    return fragment_.get();
}

VkDescriptorSetLayout VulkanShader::getDescriptorSetLayout() const {
    return descriptorSetLayout_;
}

VkDescriptorSet VulkanShader::getDescriptorSet(uint32_t frameIndex) const {
    return descriptorSets_[frameIndex % MAX_FRAMES_IN_FLIGHT];
}

uint32_t VulkanShader::getUniformBinding(const std::string& name) const { 
    auto it = uniformOffsets.find(name); 
    if (it == uniformOffsets.end()) { 
        throw std::runtime_error("Binding not found for uniform: " + name); 
    }
    
    return it->second; 
}

uint32_t VulkanShader::getTextureBinding(const std::string& name) const {
    auto it = textureNameToBinding.find(name);
    if (it == textureNameToBinding.end()){
        throw std::runtime_error("Sampler not found: " + name);
    }
    
    return it->second;
}

} // namespace jelly::graphics::vulkan
