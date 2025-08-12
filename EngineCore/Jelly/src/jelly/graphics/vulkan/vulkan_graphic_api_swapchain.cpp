#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::createSwapchain() {
    SwapChainSupportDetails support = querySwapChainSupport(physicalDevice_, surface_);

    VkSurfaceFormatKHR surfaceFmt = chooseSurfaceFormat(support.formats);
    VkPresentModeKHR present = choosePresentMode(support.presentModes);
    VkExtent2D extent = chooseSwapExtent(support.capabilities);

    uint32_t imageCount = support.capabilities.minImageCount + 1;
    if (support.capabilities.maxImageCount &&
        imageCount > support.capabilities.maxImageCount) {
        imageCount = support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR sci{};
    sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    sci.surface = surface_;
    sci.minImageCount = imageCount;
    sci.imageFormat = surfaceFmt.format;
    sci.imageColorSpace = surfaceFmt.colorSpace;
    sci.imageExtent = extent;
    sci.imageArrayLayers = 1;
    sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices idx = findQueueFamilies(physicalDevice_, surface_);
    uint32_t qfams[] = {idx.graphicsFamily.value(), idx.presentFamily.value()};

    if (idx.graphicsFamily != idx.presentFamily) {
        sci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        sci.queueFamilyIndexCount = 2;
        sci.pQueueFamilyIndices = qfams;
    }
    else {
        sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    sci.preTransform = support.capabilities.currentTransform;
    sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    sci.presentMode = present;
    sci.clipped = VK_TRUE;

    VkSwapchainKHR rawSwapchain = VK_NULL_HANDLE;
    if (vkCreateSwapchainKHR(device_, &sci, nullptr, &rawSwapchain) != VK_SUCCESS) {
        throw Exception("Failed to create swapchain");
    }

    swapchain_ = ManagedResource<VkSwapchainKHR>(
        rawSwapchain,
        [devicePtr = device_.get()](VkSwapchainKHR sc) {
            vkDestroySwapchainKHR(devicePtr, sc, nullptr);
        },
        VK_NULL_HANDLE
    );

    swapchainImageFormat = surfaceFmt.format;
    swapchainExtent = extent;

    uint32_t count = 0;
    vkGetSwapchainImagesKHR(device_, swapchain_, &count, nullptr);
    swapchainImages.resize(count);
    vkGetSwapchainImagesKHR(device_, swapchain_, &count, swapchainImages.data());
}

void VulkanGraphicAPI::recreateSwapchain()
{
    uint32_t width = 0, height = 0;
    windowProvider_->getFramebufferSize(width, height);

    while (width == 0 || height == 0) {
        windowProvider_->getFramebufferSize(width, height);
        windowProvider_->waitEvents();
    }

    vkDeviceWaitIdle(device_);

    cleanupSwapchain();

    createSwapchain();
    createImageViews();
    createRenderPass();
    createFramebuffers();
    createCommandBuffers();
    createSyncObjects();
}

void VulkanGraphicAPI::cleanupSwapchain()
{
    // TODO : Destroy framebuffers (no RAII yet)
    for (auto framebuffer : swapchainFramebuffers_) {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(device_, framebuffer, nullptr);
        }
    }
    swapchainFramebuffers_.clear();

    // TODO : Destroy image views (no RAII yet)
    for (auto view : swapchainImageViews) {
        if (view != VK_NULL_HANDLE) {
            vkDestroyImageView(device_, view, nullptr);
        }
    }
    swapchainImageViews.clear();

    swapchain_.reset();
    renderPass_.reset();
}

}