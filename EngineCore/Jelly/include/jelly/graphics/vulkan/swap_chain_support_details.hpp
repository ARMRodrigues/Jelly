#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

/// Holds all necessary swap chain support details for a physical device and surface.
struct SwapChainSupportDetails {
    /// Basic surface capabilities (min/max image count, extent, etc.).
    VkSurfaceCapabilitiesKHR capabilities;

    /// Supported surface formats (color space and pixel format).
    std::vector<VkSurfaceFormatKHR> formats;

    /// Supported presentation modes (e.g., FIFO, MAILBOX).
    std::vector<VkPresentModeKHR> presentModes;
};

}