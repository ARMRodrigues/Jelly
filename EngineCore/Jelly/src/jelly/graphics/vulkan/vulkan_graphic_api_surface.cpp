#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"

#include <iostream>

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::createSurface() {
    if (!windowProvider_) {
        throw jelly::Exception("Window system does not support Vulkan surface creation!");
    }

    VkSurfaceKHR rawSurface = windowProvider_->createVulkanSurface(instance_);

    surface_ = jelly::core::ManagedResource<VkSurfaceKHR>(
        rawSurface,
        [inst = instance_.get()](VkSurfaceKHR surf) { vkDestroySurfaceKHR(inst, surf, nullptr); }
    );
}

}