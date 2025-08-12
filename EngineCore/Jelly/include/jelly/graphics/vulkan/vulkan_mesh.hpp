#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/graphics/mesh.hpp"
#include <vulkan/vulkan.h>
#include <stdexcept> // For std::runtime_error

namespace jelly::graphics::vulkan {

/// @brief A Vulkan-specific implementation of the `Mesh` interface.
/// @details This class manages the lifecycle of Vulkan vertex and index buffers (`VkBuffer`).
/// It handles allocating GPU memory, uploading data from the CPU, and issuing the
/// necessary Vulkan commands to draw the mesh.
class JELLY_EXPORT VulkanMesh : public Mesh {
public:
    /// @brief Constructs a `VulkanMesh`.
    /// @param device The logical Vulkan device used to create resources.
    /// @param physicalDevice The physical Vulkan device used to query memory properties.
    VulkanMesh(VkDevice device, VkPhysicalDevice physicalDevice);

    /// @brief Destroys the `VulkanMesh` and cleans up all associated Vulkan resources.
    /// @details This is crucial to prevent GPU memory leaks. It frees the vertex and
    /// index buffers and their corresponding device memory.
    ~VulkanMesh() override;

    /// @brief Creates and populates the Vulkan vertex and index buffers with the given data.
    /// @details This method allocates GPU memory and copies the mesh data from the CPU to the GPU.
    /// It overrides the `Mesh::upload` method.
    /// @param vertexData A pointer to the raw vertex data.
    /// @param vertexSize The total size of the vertex data in bytes.
    /// @param indexData A pointer to the raw index data.
    /// @param indexSize The total size of the index data in bytes.
    void upload(const void* vertexData, size_t vertexSize,
                const void* indexData, size_t indexSize) override;

    /// @brief Binds the vertex and index buffers and issues a Vulkan draw command.
    /// @details This method must be called within an active Vulkan command buffer recording.
    /// It overrides the `Mesh::draw` method.
    void draw() const override;

private:
    /// @brief The logical Vulkan device.
    VkDevice device_;
    /// @brief The physical Vulkan device (GPU).
    VkPhysicalDevice physicalDevice_;
    /// @brief The Vulkan buffer holding vertex data.
    VkBuffer vertexBuffer_{};
    /// @brief The device memory allocated for the vertex buffer.
    VkDeviceMemory vertexMemory_{};
    /// @brief The Vulkan buffer holding index data.
    VkBuffer indexBuffer_{};
    /// @brief The device memory allocated for the index buffer.
    VkDeviceMemory indexMemory_{};
    /// @brief The number of indices in the index buffer, used for draw calls.
    uint32_t indexCount_{};

    /// @brief A helper function to create a Vulkan buffer and allocate its memory.
    /// @param size The desired size of the buffer in bytes.
    /// @param usage The usage flags for the buffer (e.g., vertex buffer, index buffer).
    /// @param properties The desired memory property flags (e.g., device local, host visible).
    /// @param buffer [out] A reference to the `VkBuffer` handle to be created.
    /// @param memory [out] A reference to the `VkDeviceMemory` handle to be allocated.
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer& buffer, VkDeviceMemory& memory);
    
    /// @brief Finds a suitable memory type index on the physical device.
    /// @param typeFilter A bitmask of memory types that are suitable for the buffer.
    /// @param properties The required memory property flags.
    /// @return The index of the suitable memory type.
    /// @throws std::runtime_error if a suitable memory type cannot be found.
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

} // namespace jelly::graphics::vulkan