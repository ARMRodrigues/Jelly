#include "jelly/graphics/vulkan/vulkan_mesh.hpp"
#include "jelly/graphics/graphic_context.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include <stdexcept>
#include <cstring>

namespace jelly::graphics::vulkan {

VulkanMesh::VulkanMesh(VkDevice device, VkPhysicalDevice physicalDevice)
    : device_(device), physicalDevice_(physicalDevice) {}

VulkanMesh::~VulkanMesh() = default;

void VulkanMesh::upload() {
    auto vertices = buildVertexBuffer();

    VkDeviceSize vertexSize = vertices.size() * sizeof(Vertex);

    createBuffer(vertexSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        vertexBuffer_, vertexMemory_
    );

    void* data = nullptr;
    vkMapMemory(device_, vertexMemory_.get(), 0, vertexSize, 0, &data);
    std::memcpy(data, vertices.data(), static_cast<size_t>(vertexSize));
    vkUnmapMemory(device_, vertexMemory_.get());

    VkDeviceSize indexSize = indices_.size() * sizeof(uint32_t);
    createBuffer(indexSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        indexBuffer_, indexMemory_
    );

    vkMapMemory(device_, indexMemory_.get(), 0, indexSize, 0, &data);
    std::memcpy(data, indices_.data(), static_cast<size_t>(indexSize));
    vkUnmapMemory(device_, indexMemory_.get());

    indexCount_ = static_cast<uint32_t>(indices_.size());
}

void VulkanMesh::draw() const {
    auto vulkanAPI = static_cast<VulkanGraphicAPI*>(GraphicContext::get().getAPI());
    VkCommandBuffer cmdBuffer = vulkanAPI->getCurrentCommandBuffer();

    VkBuffer vertexBuffers[] = { vertexBuffer_.get() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, indexBuffer_.get(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer, indexCount_, 1, 0, 0, 0);
}

void VulkanMesh::release()
{
    if (device_ != VK_NULL_HANDLE) {
        vertexBuffer_.reset();
        vertexMemory_.reset();
        indexBuffer_.reset();
        indexMemory_.reset();
    }
}

void VulkanMesh::createBuffer(
    VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    core::ManagedResource<VkBuffer>& buffer,
    core::ManagedResource<VkDeviceMemory>& memory
) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer rawBuffer{};
    if (vkCreateBuffer(device_, &bufferInfo, nullptr, &rawBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to create buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device_, rawBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    VkDeviceMemory rawMemory{};
    if (vkAllocateMemory(device_, &allocInfo, nullptr, &rawMemory) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate buffer memory");

    vkBindBufferMemory(device_, rawBuffer, rawMemory, 0);

    // Wrap resources in ManagedResource with proper cleanup
    buffer = core::ManagedResource<VkBuffer>(
        rawBuffer,
        [device = device_](VkBuffer buf) { vkDestroyBuffer(device, buf, nullptr); },
        VK_NULL_HANDLE
    );

    memory = core::ManagedResource<VkDeviceMemory>(
        rawMemory,
        [device = device_](VkDeviceMemory mem) { vkFreeMemory(device, mem, nullptr); },
        VK_NULL_HANDLE
    );
}

uint32_t VulkanMesh::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }

    throw std::runtime_error("failed to find suitable memory type");
}

} // namespace jelly::graphics::vulkan
