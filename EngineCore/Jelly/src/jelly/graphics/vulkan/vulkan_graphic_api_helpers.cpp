#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include <algorithm>

namespace jelly::graphics::vulkan {

SwapChainSupportDetails VulkanGraphicAPI::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    // Capacidades da superfície (ex: número min/max de imagens, tamanhos suportados)
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    // Formatos suportados (ex: VK_FORMAT_B8G8R8A8_SRGB + VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount > 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    // Modos de apresentação suportados (ex: FIFO, MAILBOX, IMMEDIATE)
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount > 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

QueueFamilyIndices VulkanGraphicAPI::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> families(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.data());

    for (uint32_t i = 0; i < count; ++i)
    {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport)
            indices.presentFamily = i;

        if (indices.IsComplete())
            break;
    }

    return indices;
}

VkSurfaceFormatKHR VulkanGraphicAPI::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats)
{
    for (const auto &f : formats)
    {
        if (f.format == VK_FORMAT_B8G8R8A8_SRGB &&
            f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return f;
    }
    return formats[0]; // fallback
}

VkPresentModeKHR VulkanGraphicAPI::choosePresentMode(const std::vector<VkPresentModeKHR> &modes)
{
    for (VkPresentModeKHR m : modes)
        if (m == VK_PRESENT_MODE_MAILBOX_KHR) // triplo buffering, suave
            return m;
    return VK_PRESENT_MODE_FIFO_KHR; // vs-sync garantido, 100% compat.
}

VkExtent2D VulkanGraphicAPI::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &caps)
{
    // Alguns SOs fixam currentExtent; use-o se válido.
    if (caps.currentExtent.width != UINT32_MAX)
        return caps.currentExtent;

    uint32_t width = 0, height = 0;
    windowProvider_->getFramebufferSize(width, height);

    VkExtent2D extent{width, height};
    extent.width = std::clamp(extent.width, caps.minImageExtent.width,
                              caps.maxImageExtent.width);
    extent.height = std::clamp(extent.height, caps.minImageExtent.height,
                               caps.maxImageExtent.height);
    return extent;
}

}