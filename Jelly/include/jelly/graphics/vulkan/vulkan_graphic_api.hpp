#pragma once

#include "queue_family_indices.hpp"
#include "swap_chain_support_details.hpp"

#include "jelly/core/managed_resource.hpp"
#include "jelly/graphics/graphic_api_interface.hpp"
#include "jelly/windowing/native_window_handle_provider_interface.hpp"
#include "jelly/windowing/vulkan_native_window_handle_provider.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <set>

namespace jelly::graphics::vulkan {

using jelly::core::ManagedResource;

/// <summary>
/// Vulkan-based implementation of the graphics API.
/// Responsible for setting up Vulkan instance, device, swapchain, and rendering pipeline.
/// </summary>
class VulkanGraphicAPI final : public GraphicAPIInterface {
public:
    /// <inheritdoc />
    void initialize() override;

    /// <inheritdoc />
    //void initialize(jelly::windowing::WindowSystemInterface* windowSystem) override;

    /// <inheritdoc />
    void beginFrame() override;

    /// <inheritdoc />
    void endFrame() override;

    /// <inheritdoc />
    void shutdown() override;

    // Setters
    void setWindowProvider(jelly::windowing::VulkanNativeWindowHandleProvider* provider);

private:
    // Window system
    jelly::windowing::VulkanNativeWindowHandleProvider* windowProvider_ = nullptr;

    // Vulkan instance and device
    ManagedResource<VkInstance> instance_;
    ManagedResource<VkPhysicalDevice> physicalDevice_;
    ManagedResource<VkDevice> device_;

    // Queues
    VkQueue graphicsQueue_ = VK_NULL_HANDLE;
    VkQueue presentQueue_ = VK_NULL_HANDLE;

    // Surface and swapchain
    ManagedResource<VkSurfaceKHR> surface_;
    ManagedResource<VkSwapchainKHR> swapchain_;
    VkFormat swapchainImageFormat{};
    VkExtent2D swapchainExtent{};
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;

    // Render pass and framebuffers
    ManagedResource<VkRenderPass> renderPass_;
    std::vector<VkFramebuffer> swapchainFramebuffers_;

    // Command pool and buffers
    ManagedResource<VkCommandPool> commandPool_;
    std::vector<VkCommandBuffer> commandBuffers;

    // Synchronization
    std::vector<VkSemaphore> imageAvailableSemaphores_;
    std::vector<VkSemaphore> renderFinishedSemaphores_;
    std::vector<VkFence> inFlightFences_;

    // Frame state
    size_t currentFrame = 0;
    uint32_t currentImageIndex = 0;
    static constexpr int MaxFramesInFlight = 2;

    // Initialization steps
    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapchain();
    void createImageViews();
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
    void recreateSwapchain();
    void cleanupSwapchain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    // Helper functions
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& modes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps);
};

}
