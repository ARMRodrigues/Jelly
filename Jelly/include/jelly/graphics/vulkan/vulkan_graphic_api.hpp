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

/// @brief Vulkan-based implementation of the graphics API.
/// 
/// Responsible for initializing Vulkan instance, device, swapchain, render pass, and command infrastructure.
/// Handles frame rendering and synchronization.
class VulkanGraphicAPI final : public GraphicAPIInterface {
public:
    /// @brief Initializes the Vulkan graphics backend.
    void initialize() override;

    /// @brief Begins a new frame (acquires image, begins command buffer recording, etc.).
    void beginFrame() override;

    /// @brief Ends the current frame (submits commands and presents the image).
    void endFrame() override;

    /// @brief Cleans up all Vulkan resources.
    void shutdown() override;

    /// @brief Sets the window provider used for Vulkan surface creation.
    /// @param provider A pointer to a VulkanNativeWindowHandleProvider.
    void setWindowProvider(jelly::windowing::VulkanNativeWindowHandleProvider* provider);

    /// @brief Returns the logical Vulkan device.
    /// @return The Vulkan device handle.
    VkDevice getDevice() const { return device_.get(); }

    /// @brief Returns the Vulkan render pass used for drawing.
    /// @return The Vulkan render pass handle.
    VkRenderPass getRenderPass() const { return renderPass_.get(); }

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
    /// @brief Creates the Vulkan instance.
    void createInstance();

    /// @brief Creates the Vulkan surface from the window.
    void createSurface();

    /// @brief Selects a suitable physical device (GPU).
    void pickPhysicalDevice();

    /// @brief Creates the logical device and retrieves queue handles.
    void createLogicalDevice();

    /// @brief Creates the swapchain based on surface capabilities.
    void createSwapchain();

    /// @brief Creates image views for each swapchain image.
    void createImageViews();

    /// @brief Creates the Vulkan render pass.
    void createRenderPass();

    /// @brief Creates framebuffers for all swapchain image views.
    void createFramebuffers();

    /// @brief Allocates the command pool.
    void createCommandPool();

    /// @brief Allocates command buffers from the command pool.
    void createCommandBuffers();

    /// @brief Initializes synchronization primitives (semaphores and fences).
    void createSyncObjects();

    /// @brief Recreates swapchain when the window is resized or invalidated.
    void recreateSwapchain();

    /// @brief Destroys swapchain-dependent resources.
    void cleanupSwapchain();

    /// @brief Records commands to draw into the specified command buffer.
    /// @param commandBuffer The command buffer to record into.
    /// @param imageIndex Index of the swapchain image being rendered to.
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    // Helper functions

    /// @brief Queries swapchain support details for a given physical device.
    /// @param device The Vulkan physical device.
    /// @param surface The Vulkan surface.
    /// @return SwapChainSupportDetails struct with capabilities.
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    /// @brief Finds queue families for graphics and presentation.
    /// @param device The Vulkan physical device.
    /// @param surface The Vulkan surface.
    /// @return QueueFamilyIndices struct with queue family indices.
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    /// @brief Chooses the best surface format from available options.
    /// @param formats List of supported surface formats.
    /// @return The chosen VkSurfaceFormatKHR.
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);

    /// @brief Chooses the best present mode (e.g., FIFO, Mailbox).
    /// @param modes List of supported present modes.
    /// @return The chosen VkPresentModeKHR.
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& modes);

    /// @brief Chooses the swapchain extent (resolution) based on capabilities.
    /// @param caps Surface capabilities.
    /// @return The chosen VkExtent2D.
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps);
};

} // namespace jelly::graphics::vulkan
