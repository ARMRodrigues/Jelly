#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::createSyncObjects() {
    for (VkFence f : inFlightFences_) {
        if (f) vkDestroyFence(device_, f, nullptr);
    }

    for (VkSemaphore s : imageAvailableSemaphores_)
        if (s) vkDestroySemaphore(device_, s, nullptr);
    for (VkSemaphore s : renderFinishedSemaphores_)
        if (s) vkDestroySemaphore(device_, s, nullptr);

    inFlightFences_.clear();
    imageAvailableSemaphores_.clear();
    renderFinishedSemaphores_.clear();
    imagesInFlight_.clear();

    inFlightFences_.resize(maxFramesInFlight_, VK_NULL_HANDLE);
    imageAvailableSemaphores_.resize(maxFramesInFlight_, VK_NULL_HANDLE);
    renderFinishedSemaphores_.resize(maxFramesInFlight_, VK_NULL_HANDLE);
    imagesInFlight_.resize(swapchainImages_.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (std::size_t i = 0; i < inFlightFences_.size(); ++i) {
        if (vkCreateSemaphore(device_, &semInfo, nullptr, &imageAvailableSemaphores_[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device_, &semInfo, nullptr, &renderFinishedSemaphores_[i]) != VK_SUCCESS ||
            vkCreateFence(device_, &fenceInfo, nullptr, &inFlightFences_[i]) != VK_SUCCESS)
        {
            throw Exception("Failed to create sync objects!");
        }
    }
}

}