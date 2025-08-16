#pragma once

#include "jelly/jelly_export.hpp"

#include <string>
#include <vector>
#include <cstdint>

namespace jelly::graphics {

/// @brief Representa uma imagem carregada na memória do host (CPU).
/// Contém largura, altura e dados RGBA.
class JELLY_EXPORT Image {
public:
    Image() = default;

    /// @brief Carrega um arquivo .data para memória.
    /// @param path Caminho para o arquivo .data
    /// @throws std::runtime_error se o arquivo não puder ser lido ou tiver formato inválido.
    explicit Image(const std::string& path);

    uint32_t getWidth() const { return width_; }
    uint32_t getHeight() const { return height_; }
    const std::vector<uint8_t>& getPixels() const { return pixels_; }

private:
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::vector<uint8_t> pixels_;
};

} // namespace jelly::graphics
