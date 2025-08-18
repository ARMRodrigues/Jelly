#include "jelly/graphics/image.hpp"
#include <fstream>
#include <stdexcept>

namespace jelly::graphics {

Image::Image(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Falha ao abrir arquivo de imagem: " + path);
    }

    // Lê largura
    file.read(reinterpret_cast<char*>(&width_), sizeof(uint32_t));
    // Lê altura
    file.read(reinterpret_cast<char*>(&height_), sizeof(uint32_t));

    if (width_ == 0 || height_ == 0) {
        throw std::runtime_error("Imagem inválida (dimensões zero).");
    }

    size_t pixelCount = static_cast<size_t>(width_) * static_cast<size_t>(height_) * 4;
    pixels_.resize(pixelCount);

    file.read(reinterpret_cast<char*>(pixels_.data()), pixelCount);

    if (!file) {
        throw std::runtime_error("Erro ao ler dados de pixel: " + path);
    }
}

} // namespace jelly::graphics
