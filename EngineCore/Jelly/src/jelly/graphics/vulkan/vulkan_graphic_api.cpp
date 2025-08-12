#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/error.hpp"
#include "jelly/exception.hpp"

namespace jelly::graphics::vulkan {

using jelly::Exception;
using jelly::Error;

void VulkanGraphicAPI::initialize() {

    try {
        createInstance();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createSurface();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        pickPhysicalDevice();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createLogicalDevice();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createSwapchain();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createImageViews();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createRenderPass();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createFramebuffers();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createCommandPool();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createCommandBuffers();
    } catch (const Exception& e) {
        Error::Print(e);
    }

    try {
        createSyncObjects();
    } catch (const Exception& e) {
        Error::Print(e);
    }
}

void VulkanGraphicAPI::beginFrame() {
    VkResult result = vkAcquireNextImageKHR(
            device_,
            swapchain_,
            UINT64_MAX,
            imageAvailableSemaphores_[currentFrame],
            VK_NULL_HANDLE,
            &currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw Exception("Failed to acquire swap chain image!");
    }

    if (imagesInFlight_[currentImageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(device_, 1, &imagesInFlight_[currentImageIndex], VK_TRUE, UINT64_MAX);
    }

    vkResetFences(device_, 1, &inFlightFences_[currentFrame]);

    beginCommandBuffer(commandBuffers[currentImageIndex], currentImageIndex);
}

void VulkanGraphicAPI::endFrame() {
    endCommandBuffer(commandBuffers[currentImageIndex]);

    VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores_[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentImageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores_[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue_, 1, &submitInfo, inFlightFences_[currentFrame]) != VK_SUCCESS) {
        throw Exception("Failed to submit draw command buffer!");
    }
    
    imagesInFlight_[currentImageIndex] = inFlightFences_[currentFrame];

    VkSwapchainKHR const* rawSwapchainPtr = std::addressof(swapchain_.ref());

    VkPresentInfoKHR presentInfo{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = rawSwapchainPtr;
    presentInfo.pImageIndices = &currentImageIndex;

    VkResult result = vkQueuePresentKHR(presentQueue_, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        recreateSwapchain();
    } else if (result != VK_SUCCESS) {
        throw Exception("Failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MaxFramesInFlight;
}

void VulkanGraphicAPI::shutdown() {
    imageAvailableSemaphores_.clear();
    renderFinishedSemaphores_.clear();
    inFlightFences_.clear();

    for (VkFramebuffer fb : swapchainFramebuffers_) {
        if (fb)
            vkDestroyFramebuffer(device_, fb, nullptr);
    }
    swapchainFramebuffers_.clear();

    for (VkImageView view : swapchainImageViews) {
        if (view)
            vkDestroyImageView(device_, view, nullptr);
    }
    swapchainImageViews.clear();

    swapchain_.reset();
    renderPass_.reset();

    commandPool_.reset();
    commandBuffers.clear();

    device_.reset();

    surface_.reset();

    instance_.reset();

    physicalDevice_.reset();

    windowProvider_ = nullptr;
}

void VulkanGraphicAPI::setWindowProvider(
	jelly::windowing::VulkanNativeWindowHandleProvider *provider) {
    this->windowProvider_ = provider;
}

}