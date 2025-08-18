#pragma once

#include "image.hpp"

#include "jelly/jelly_export.hpp"

#include <memory>

namespace jelly::graphics {

/// @brief Interface para texturas na GPU
class JELLY_EXPORT TextureInterface {
public:
    virtual ~TextureInterface() = default;

    /// @brief Faz upload da imagem para GPU
    virtual void upload(const Image& image) = 0;

    /// @brief Obtém largura da textura
    virtual uint32_t getWidth() const = 0;

    /// @brief Obtém altura da textura
    virtual uint32_t getHeight() const = 0;
};

using TextureHandle = std::shared_ptr<TextureInterface>;

} // namespace jelly::graphics
