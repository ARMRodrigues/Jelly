#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"

#include <iostream>

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice_, surface_);

    VkCommandPoolCreateInfo poolInfo {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    VkCommandPool rawCommandPool = VK_NULL_HANDLE;

    if (vkCreateCommandPool(device_, &poolInfo, nullptr, &rawCommandPool) != VK_SUCCESS) {
        throw Exception("Failed to create command pool!");
    }

    commandPool_ = ManagedResource<VkCommandPool>(
        rawCommandPool,
        [devicePtr = device_.get()](VkCommandPool cp) {
            vkDestroyCommandPool(devicePtr, cp, nullptr);
        },
        VK_NULL_HANDLE
    );
}

}