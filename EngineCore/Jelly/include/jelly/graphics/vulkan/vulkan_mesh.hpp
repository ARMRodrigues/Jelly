#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/graphics/mesh.hpp"
#include "jelly/core/managed_resource.hpp"
#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

/// Vulkan implementation of Mesh.
/// Manages vertex/index buffers and their memory.
class JELLY_EXPORT VulkanMesh : public Mesh {
public:
    VulkanMesh(VkDevice device, VkPhysicalDevice physicalDevice);
    ~VulkanMesh() override;

    void upload(const void* vertexData, size_t vertexSize,
                const void* indexData, size_t indexSize) override;

    void draw() const override;

private:
    VkDevice device_;
    VkPhysicalDevice physicalDevice_;
    uint32_t indexCount_{0};

    // Managed Vulkan buffers and memory
    core::ManagedResource<VkBuffer> vertexBuffer_{};
    core::ManagedResource<VkDeviceMemory> vertexMemory_{};
    core::ManagedResource<VkBuffer> indexBuffer_{};
    core::ManagedResource<VkDeviceMemory> indexMemory_{};

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        core::ManagedResource<VkBuffer>& buffer,
        core::ManagedResource<VkDeviceMemory>& memory
    );

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

} // namespace jelly::graphics::vulkan
