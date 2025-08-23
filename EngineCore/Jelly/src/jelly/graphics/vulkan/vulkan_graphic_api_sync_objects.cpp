#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::createSyncObjects() {
    for (auto f : inFlightFences_) if (f) vkDestroyFence(device_, f, nullptr);
    for (auto s : imageAvailableSemaphores_) if (s) vkDestroySemaphore(device_, s, nullptr);
    for (auto s : renderFinishedSemaphoresPerImage_) if (s) vkDestroySemaphore(device_, s, nullptr);

    inFlightFences_.clear();
    imageAvailableSemaphores_.clear();
    renderFinishedSemaphoresPerImage_.clear();
    imagesInFlight_.clear();

    inFlightFences_.resize(maxFramesInFlight_, VK_NULL_HANDLE);
    imageAvailableSemaphores_.resize(maxFramesInFlight_, VK_NULL_HANDLE);
    renderFinishedSemaphoresPerImage_.resize(swapchainImages_.size(), VK_NULL_HANDLE);
    imagesInFlight_.resize(swapchainImages_.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < (size_t)maxFramesInFlight_; ++i) {
        if (vkCreateSemaphore(device_, &semInfo, nullptr, &imageAvailableSemaphores_[i]) != VK_SUCCESS ||
            vkCreateFence(device_, &fenceInfo, nullptr, &inFlightFences_[i]) != VK_SUCCESS) {
            throw Exception("Failed to create per-frame sync objects");
        }
    }

    for (size_t i = 0; i < swapchainImages_.size(); ++i) {
        if (vkCreateSemaphore(device_, &semInfo, nullptr, &renderFinishedSemaphoresPerImage_[i]) != VK_SUCCESS) {
            throw Exception("Failed to create per-image renderFinished semaphore");
        }
    }
}

}