#pragma once

#include "queue_family_indices.hpp"
#include "swap_chain_support_details.hpp"

#include "jelly/jelly_export.hpp"
#include "jelly/core/managed_resource.hpp"
#include "jelly/graphics/graphic_api_interface.hpp"
#include "jelly/windowing/native_window_handle_provider_interface.hpp"
#include "jelly/windowing/vulkan_native_window_handle_provider.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <set>

namespace jelly::graphics::vulkan {

using jelly::core::ManagedResource;

/// @brief Vulkan-based implementation of the graphics API.
/// Responsible for initializing Vulkan instance, device, swapchain, render pass, and command infrastructure.
/// Handles frame rendering and synchronization.
class JELLY_EXPORT VulkanGraphicAPI final : public GraphicAPIInterface {
public:
    /// @brief Initializes the Vulkan graphics backend.
    void initialize() override;

    /// @brief Begins a new frame (acquires image, begins command buffer recording, etc.).
    void beginFrame() override;

    /// @brief Begins recording commands into a specific command buffer.
    /// @param commandBuffer The command buffer to start recording into.
    /// @param imageIndex The index of the current swapchain image for rendering context.
    void beginCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    /// @brief Ends the recording of a command buffer.
    /// @param commandBuffer The command buffer to stop recording.
    void endCommandBuffer(VkCommandBuffer commandBuffer);

    /// @brief Ends the current frame (submits commands and presents the image).
    void endFrame() override;

    /// @brief Cleans up all Vulkan resources.
    void shutdown() override;

    /// @brief Sets the window provider used for Vulkan surface creation.
    /// @param provider A pointer to a VulkanNativeWindowHandleProvider.
    void setWindowProvider(jelly::windowing::VulkanNativeWindowHandleProvider* provider);

    /// @brief Returns the logical Vulkan device.
    VkDevice getDevice() const { return device_.get(); }

    /// @brief Returns the Vulkan render pass used for drawing.
    VkRenderPass getRenderPass() const { return renderPass_.get(); }

    /// @brief Returns the Vulkan physical device.
    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice_.get(); }

    /// @brief Returns the command buffer currently being recorded.
    VkCommandBuffer getCurrentCommandBuffer() const { return commandBuffers_[currentImageIndex_]; }

    /// @brief Gets the extent (resolution) of the swapchain.
    VkExtent2D getSwapchainExtent() const { return swapchainExtent_; }

private:
    // === Window system ===
    jelly::windowing::VulkanNativeWindowHandleProvider* windowProvider_ = nullptr;

    // === Vulkan instance and device ===
    ManagedResource<VkInstance> instance_;
    ManagedResource<VkPhysicalDevice> physicalDevice_;
    ManagedResource<VkDevice> device_;

    // === Queues ===
    VkQueue graphicsQueue_ = VK_NULL_HANDLE;
    VkQueue presentQueue_ = VK_NULL_HANDLE;

    // === Surface and swapchain ===
    ManagedResource<VkSurfaceKHR> surface_;
    ManagedResource<VkSwapchainKHR> swapchain_;
    VkFormat swapchainImageFormat_{};
    VkExtent2D swapchainExtent_{};
    std::vector<VkImage> swapchainImages_; // Vulkan destroys via swapchain destroy
    std::vector<VkImageView> swapchainImageViews_;

    // === Render pass and framebuffers ===
    ManagedResource<VkRenderPass> renderPass_;
    std::vector<VkFramebuffer> swapchainFramebuffers_;

    // === Command pool and buffers ===
    ManagedResource<VkCommandPool> commandPool_;
    std::vector<VkCommandBuffer> commandBuffers_; // Freed with command pool

    // === Synchronization ===
    std::vector<VkSemaphore> imageAvailableSemaphores_;
    std::vector<VkSemaphore> renderFinishedSemaphores_;
    std::vector<VkFence> inFlightFences_;
    std::vector<VkFence> imagesInFlight_;

    // === Frame state ===
    size_t currentFrame_ = 0;
    uint32_t currentImageIndex_ = 0;
    static constexpr int maxFramesInFlight_ = 2;

    // === Initialization steps ===
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

    // === Helper functions ===
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& modes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps);
};

} // namespace jelly::graphics::vulkan
