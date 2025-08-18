#pragma once

#include "jelly/jelly_export.hpp"

#include "jelly/graphics/graphic_context.hpp"
#include "jelly/graphics/texture_interface.hpp"

#include "jelly/graphics/vulkan/vulkan_texture.hpp"

namespace jelly::graphics {

class JELLY_EXPORT TextureFactory {
public:
    static TextureHandle create(const Image& image) {
        switch (GraphicContext::get().getAPIType()) {
            case core::GraphicAPIType::Vulkan: {
                auto api = static_cast<vulkan::VulkanGraphicAPI*>(GraphicContext::get().getAPI());
                auto texture = std::make_shared<vulkan::VulkanTexture>(api);
                texture->upload(image);
                return texture;
            }
            default:
                throw std::runtime_error("Unsupported graphics API in TextureFactory");
        }
    }
};

} // namespace jelly::graphics
