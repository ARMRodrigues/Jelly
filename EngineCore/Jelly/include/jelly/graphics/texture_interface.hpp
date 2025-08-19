#pragma once

#include "image.hpp"

#include "jelly/jelly_export.hpp"

#include <memory>

namespace jelly::graphics {

/// @brief Interface for GPU texture resources
///
/// Defines core operations for texture management
/// across different graphics APIs (Vulkan, OpenGL, etc.)
class JELLY_EXPORT TextureInterface {
public:
    virtual ~TextureInterface() = default;

    /// @brief Uploads image data to GPU memory
    /// @param image Image data to upload
    virtual void upload(const Image& image) = 0;

    /// @brief Gets texture width in pixels
    /// @return Texture width
    virtual uint32_t getWidth() const = 0;

    /// @brief Gets texture height in pixels
    /// @return Texture height
    virtual uint32_t getHeight() const = 0;

    /// @brief Releases GPU resources
    /// @note Must be called before object destruction
    virtual void release() = 0;
};

/// @brief Managed handle to TextureInterface instances
using TextureHandle = std::shared_ptr<TextureInterface>;

} // namespace jelly::graphics