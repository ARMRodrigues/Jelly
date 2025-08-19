#pragma once

#include "jelly/jelly_export.hpp"

#include "jelly/core/managed_resource.hpp"

#include "jelly/graphics/mesh.hpp"

#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

/// @brief Vulkan implementation of a renderable mesh
///
/// Manages vertex/index buffers and their associated GPU memory.
/// Uses ManagedResource for automatic Vulkan resource cleanup.
class JELLY_EXPORT VulkanMesh : public Mesh {
public:
    /// @brief Constructs a Vulkan mesh instance
    /// @param device Logical Vulkan device
    /// @param physicalDevice Physical Vulkan device
    VulkanMesh(VkDevice device, VkPhysicalDevice physicalDevice);
    ~VulkanMesh() override;

    /// @brief Uploads vertex and index data to GPU
    /// @param vertexData Pointer to vertex data
    /// @param vertexSize Size of vertex data in bytes
    /// @param indexData Pointer to index data
    /// @param indexSize Size of index data in bytes
    void upload(
        const void* vertexData,
        size_t vertexSize,
        const void* indexData,
        size_t indexSize
    ) override;

    /// @brief Issues draw commands for this mesh
    void draw() const override;

    /// @brief Releases all Vulkan resources immediately
    void release() override;

private:
    VkDevice device_;
    VkPhysicalDevice physicalDevice_;
    uint32_t indexCount_{0};

    // Managed Vulkan resources
    core::ManagedResource<VkBuffer> vertexBuffer_{};
    core::ManagedResource<VkDeviceMemory> vertexMemory_{};
    core::ManagedResource<VkBuffer> indexBuffer_{};
    core::ManagedResource<VkDeviceMemory> indexMemory_{};

    /// @brief Creates a Vulkan buffer with allocated memory
    /// @param size Buffer size in bytes
    /// @param usage Buffer usage flags
    /// @param properties Memory property flags
    /// @param buffer Output buffer handle
    /// @param memory Output memory handle
    void createBuffer(
        VkDeviceSize size, VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        core::ManagedResource<VkBuffer>& buffer,
        core::ManagedResource<VkDeviceMemory>& memory
    );

    /// @brief Finds suitable memory type for allocation
    /// @param typeFilter Supported memory types
    /// @param properties Desired memory properties
    /// @return Index of suitable memory type
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

} // namespace jelly::graphics::vulkan
