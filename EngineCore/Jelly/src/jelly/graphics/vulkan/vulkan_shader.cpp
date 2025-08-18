#include "jelly/graphics/vulkan/vulkan_shader.hpp"

#include <stdexcept>
#include <string.h>

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

    createUniformBuffer();
    createDescriptorSetLayout();
    createDescriptorPool();
    allocateDescriptorSet();
    updateDescriptorSet();
}

void VulkanShader::createUniformBuffer() {
    VkDevice device = api_->getDevice();
    VkPhysicalDevice physicalDevice = api_->getPhysicalDevice();

    VkDeviceSize bufferSize = sizeof(ShaderUniforms);

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer;
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create uniform buffer");
    }

    uniformBuffer_ = jelly::core::ManagedResource<VkBuffer>(
        buffer,
        [device](VkBuffer b) { vkDestroyBuffer(device, b, nullptr); },
        VK_NULL_HANDLE
    );

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, uniformBuffer_.get(), &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    uint32_t memoryTypeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((memRequirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags &
             (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) ==
            (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            memoryTypeIndex = i;
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

    uniformBufferMemory_ = jelly::core::ManagedResource<VkDeviceMemory>(
        memory,
        [device](VkDeviceMemory m) { vkFreeMemory(device, m, nullptr); },
        VK_NULL_HANDLE
    );

    vkBindBufferMemory(device, uniformBuffer_.get(), uniformBufferMemory_.get(), 0);
}

void VulkanShader::createDescriptorSetLayout() {
    VkDevice device = api_->getDevice();

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // view/proj/model no vertex
    uboLayoutBinding.pImmutableSamplers = nullptr;

    // Binding 1 → Combined Image Sampler
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
    poolSizes[0].descriptorCount = 1;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;

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


void VulkanShader::allocateDescriptorSet() {
    VkDevice device = api_->getDevice();

    VkDescriptorSetLayout layoutHandle = descriptorSetLayout_.get(); // pega o valor
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool_.get();
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layoutHandle; // aqui passamos o endereço de uma variável real

    if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet_) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor set");
    }
}


void VulkanShader::updateDescriptorSet() {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffer_.get();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ShaderUniforms);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet_;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(api_->getDevice(), 1, &descriptorWrite, 0, nullptr);
}

void VulkanShader::bind() {
    // Vulkan does not need explicit bind here
}

void VulkanShader::unbind() {
    // Vulkan does not need explicit unbind
}

void VulkanShader::release()
{
    if (api_ && api_->getDevice() != VK_NULL_HANDLE) { 
        fragment_.reset(); 
        vertex_.reset(); 
        
        // ManagedResource vai chamar vkDestroyBuffer, vkFreeMemory, vkDestroyDescriptorSetLayout, vkDestroyDescriptorPool 
        uniformBuffer_.reset(); 
        uniformBufferMemory_.reset(); 
        descriptorSetLayout_.reset(); 
        descriptorPool_.reset(); // descriptorSet_ não precisa destruir explicitamente, é liberado junto com o pool 
        
        descriptorSet_ = VK_NULL_HANDLE;
    }
}

void VulkanShader::setUniformVec3(const char* name, const float* vec) {
    // TODO: implement uniform setting (push constants or descriptor sets)
}

void VulkanShader::setUniformMat4(const char* name, const float* matrix) {
    if (strcmp(name, "model") == 0) {
        memcpy(uniforms_.model, matrix, sizeof(float) * 16);
    } else if (strcmp(name, "view") == 0) {
        memcpy(uniforms_.view, matrix, sizeof(float) * 16);
    } else if (strcmp(name, "projection") == 0) {
        memcpy(uniforms_.projection, matrix, sizeof(float) * 16);
    }

    void* data;
    vkMapMemory(api_->getDevice(), uniformBufferMemory_.get(), 0, sizeof(ShaderUniforms), 0, &data);
    memcpy(data, &uniforms_, sizeof(ShaderUniforms));
    vkUnmapMemory(api_->getDevice(), uniformBufferMemory_.get());
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

VkDescriptorSet VulkanShader::getDescriptorSet() const {
    return descriptorSet_;
}


} // namespace jelly::graphics::vulkan
