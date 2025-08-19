#include "jelly/graphics/texture_factory.hpp"
#include <stdexcept>

namespace jelly::graphics {

std::vector<std::weak_ptr<TextureInterface>> TextureFactory::textures_;
std::mutex TextureFactory::mutex_;

TextureHandle TextureFactory::create(const Image& image) {
    switch (GraphicContext::get().getAPIType()) {
        case core::GraphicAPIType::Vulkan: {
            auto api = static_cast<vulkan::VulkanGraphicAPI*>(GraphicContext::get().getAPI());
            auto texture = std::make_shared<vulkan::VulkanTexture>(api);
            texture->upload(image);
            registerTexture(texture);
            return texture;
        }
        default:
            throw std::runtime_error("Unsupported graphics API in TextureFactory");
    }
}

void TextureFactory::releaseAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& weakTex : textures_) {
        if (auto tex = weakTex.lock()) {
            tex->release();
        }
    }
    textures_.clear();
}

void TextureFactory::registerTexture(const TextureHandle& texture) {
    std::lock_guard<std::mutex> lock(mutex_);
    textures_.push_back(texture);
}

} // namespace jelly::graphics
