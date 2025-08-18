#pragma once

#include "shader_interface.hpp"
#include "texture_interface.hpp"

#include "jelly/jelly_export.hpp"

#include <memory>

namespace jelly::graphics {

/// @brief Types of texture maps supported by materials
enum class TextureType { Albedo, Normal };

/// @brief Interface for GPU materials used in rendering.
/// 
/// Combines a shader with associated resources (textures, uniforms, etc.)
/// to define surface properties for rendered objects.
class JELLY_EXPORT MaterialInterface {
public:
    /// @brief Constructs a material with the given shader.
    /// @param shader The shader program this material will use.
    explicit MaterialInterface(std::shared_ptr<ShaderInterface> shader)
        : shader_(std::move(shader)) {}

    /// @brief Binds the material for rendering (activates shader and resources).
    virtual void bind() = 0;

    /// @brief Sets the albedo (base color) texture
    /// @param texture The texture to use as albedo map
    virtual void setAlbedoTexture(std::shared_ptr<TextureInterface> texture) = 0;

    /// @brief Unbinds the material (if required by the graphics API).
    virtual void unbind() = 0;

    /// @brief Sets a 3-component vector uniform value.
    /// @param name Name of the uniform variable in the shader.
    /// @param vec Pointer to float data (3 consecutive elements).
    virtual void setVec3(const char* name, const float* vec) = 0;

    /// @brief Sets a 4x4 matrix uniform value.
    /// @param name Name of the uniform variable in the shader.
    /// @param matrix Pointer to float data (16 consecutive elements, column-major).
    virtual void setMat4(const char* name, const float* matrix) = 0;

    /// @brief Gets the underlying shader program.
    /// @return Shared pointer to the associated shader interface.
    std::shared_ptr<ShaderInterface> getShader() const { return shader_; }

private:
    std::shared_ptr<ShaderInterface> shader_;
};

/// @brief Handle type for material resources.
using MaterialHandle = std::shared_ptr<MaterialInterface>;

} // namespace jelly::graphics