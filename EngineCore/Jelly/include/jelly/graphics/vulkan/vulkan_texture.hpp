#pragma once

#include "vulkan_graphic_api.hpp"

#include "jelly/jelly_export.hpp"

#include "jelly/core/managed_resource.hpp"

#include "jelly/graphics/texture_interface.hpp"

#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

/// @brief Creates a Vulkan texture instance
/// @param api Pointer to the Vulkan graphics API instance
class JELLY_EXPORT VulkanTexture final : public TextureInterface {
public:
    explicit VulkanTexture(VulkanGraphicAPI* api);
    ~VulkanTexture() override;

    /// @brief Uploads image data to GPU memory
    /// @param image The image data to upload
    void upload(const Image& image) override;

    /// @brief Gets the texture width in pixels
    /// @return The width of the texture in pixels
    uint32_t getWidth() const override { return width_; }

    /// @brief Gets the texture height in pixels
    /// @return The height of the texture in pixels
    uint32_t getHeight() const override { return height_; }

    /// @brief Releases GPU resources
    /// @note Must be called before object destruction
    void release() override;

    /// @brief Gets the Vulkan image handle
    VkImage getVkImage() const { return image_.get(); }

    /// @brief Gets the Vulkan image view handle
    VkImageView getVkImageView() const { return imageView_.get(); }

    /// @brief Gets the Vulkan sampler handle
    VkSampler getVkSampler() const { return sampler_.get(); }

private:
    VulkanGraphicAPI* api_ = nullptr;

    uint32_t width_  = 0;
    uint32_t height_ = 0;

    jelly::core::ManagedResource<VkImage>        image_;
    jelly::core::ManagedResource<VkDeviceMemory> imageMemory_;
    jelly::core::ManagedResource<VkImageView>    imageView_;
    jelly::core::ManagedResource<VkSampler>      sampler_;

    /// @brief Copies data from buffer to image
    /// @param buffer Source buffer containing texture data
    /// @param image Destination image
    /// @param width Texture width in pixels
    /// @param height Texture height in pixels
    void copyBufferToImage(
    VkBuffer buffer,
    VkImage image,
    uint32_t width,
    uint32_t height);

    /// @brief Transitions image between layouts
    /// @param image The image to transition
    /// @param format Image format
    /// @param oldLayout Current image layout
    /// @param newLayout Desired image layout
    void transitionImageLayout(
    VkImage image,
    VkFormat format,
    VkImageLayout oldLayout,
    VkImageLayout newLayout);

};

} // namespace jelly::graphics::vulkan
