#pragma once

#include "jelly/graphics/texture_interface.hpp"
#include "jelly/core/managed_resource.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include <vulkan/vulkan.h>

namespace jelly::graphics::vulkan {

/// @brief Implementação Vulkan de textura 2D.
class VulkanTexture final : public TextureInterface {
public:
    explicit VulkanTexture(VulkanGraphicAPI* api);
    ~VulkanTexture() override;

    /// @brief Envia a imagem para a GPU.
    void upload(const Image& image) override;

    uint32_t getWidth()  const override { return width_; }
    uint32_t getHeight() const override { return height_; }

    /// @brief Retorna o handle Vulkan da imagem.
    VkImage getVkImage() const { return image_.get(); }

    /// @brief Retorna o handle Vulkan da image view.
    VkImageView getVkImageView() const { return imageView_.get(); }

    /// @brief Retorna o handle Vulkan do sampler.
    VkSampler getVkSampler() const { return sampler_.get(); }

private:
    VulkanGraphicAPI* api_ = nullptr;

    uint32_t width_  = 0;
    uint32_t height_ = 0;

    jelly::core::ManagedResource<VkImage>        image_;
    jelly::core::ManagedResource<VkDeviceMemory> imageMemory_;
    jelly::core::ManagedResource<VkImageView>    imageView_;
    jelly::core::ManagedResource<VkSampler>      sampler_;

    void copyBufferToImage(
    VkBuffer buffer,
    VkImage image,
    uint32_t width,
    uint32_t height);

    void transitionImageLayout(
    VkImage image,
    VkFormat format,
    VkImageLayout oldLayout,
    VkImageLayout newLayout);

};

} // namespace jelly::graphics::vulkan
