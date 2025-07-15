#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"
#include "jelly/graphics/vulkan/queue_family_indices.hpp"

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw jelly::Exception("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

    for (const auto &device : devices) {
        QueueFamilyIndices indices = findQueueFamilies(device, surface_);

        bool extensionsSupported = false;
        {
            uint32_t extCount = 0;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr);
            std::vector<VkExtensionProperties> availableExtensions(extCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, availableExtensions.data());

            std::set<std::string> requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
            for (const auto &ext : availableExtensions)
                requiredExtensions.erase(ext.extensionName);

            extensionsSupported = requiredExtensions.empty();
        }

        bool swapchainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface_);
            swapchainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        if (indices.IsComplete() && extensionsSupported && swapchainAdequate) {
            physicalDevice_ = ManagedResource<VkPhysicalDevice>(
                device,
                [](VkPhysicalDevice) { /* no cleanup */ },
                VK_NULL_HANDLE
            );
            break;
        }
    }

    if (physicalDevice_ == VK_NULL_HANDLE) {
        throw jelly::Exception("Failed to find a suitable GPU with Vulkan support and swapchain capabilities!");
    }
}

}