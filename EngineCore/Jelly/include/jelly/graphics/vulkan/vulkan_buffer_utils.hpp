#pragma once
#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

class VulkanBufferUtils {
public:
    static void createBuffer(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& memory
    );

    static void copyBuffer(
        VkDevice device,
        VkCommandPool commandPool,
        VkQueue graphicsQueue,
        VkBuffer srcBuffer,
        VkBuffer dstBuffer,
        VkDeviceSize size
    );

    static uint32_t findMemoryType(
        VkPhysicalDevice physicalDevice,
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    );

};

} // namespace jelly::graphics::vulkan
