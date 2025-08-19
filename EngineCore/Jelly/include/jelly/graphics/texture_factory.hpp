#pragma once

#include "graphic_context.hpp"
#include "texture_interface.hpp"

#include "vulkan/vulkan_texture.hpp"

#include "jelly/jelly_export.hpp"

#include <memory>
#include <mutex>
#include <vector>

namespace jelly::graphics {

/// @brief Factory for creating API-specific texture instances
///
/// Manages texture creation and lifecycle across different graphics APIs.
/// Automatically selects the appropriate implementation based on current context.
class JELLY_EXPORT TextureFactory {
public:
    /// @brief Creates a texture from image data
    /// @param image Source image data to create texture from
    /// @return Handle to the created texture
    /// @throws std::runtime_error if graphics API is unsupported
    static TextureHandle create(const Image& image);

    /// @brief Releases all managed texture resources
    /// @note Must be called before graphics device destruction
    static void releaseAll();

private:
    /// @brief Registers a texture for tracking
    /// @param texture Texture handle to register
    static void registerTexture(const TextureHandle& texture);

    static std::vector<std::weak_ptr<TextureInterface>> textures_;
    static std::mutex mutex_;
};

} // namespace jelly::graphics