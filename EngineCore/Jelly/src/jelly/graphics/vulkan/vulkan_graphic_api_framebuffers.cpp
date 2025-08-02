#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"

#include <iostream>

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::createFramebuffers() {
    swapchainFramebuffers_.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); ++i) {
        VkImageView attachments[] = {
            swapchainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass_;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapchainExtent.width;
        framebufferInfo.height = swapchainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device_, &framebufferInfo, nullptr, &swapchainFramebuffers_[i]) != VK_SUCCESS) {
            throw Exception("Failed to create framebuffer!");
        }
    }
}

}