#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/graphics/shader_interface.hpp"
#include <memory>

namespace jelly::graphics {

/// @brief Represents a GPU material used for rendering.
///
/// Wraps a shader and future resource bindings (textures, uniforms, etc.).
class JELLY_EXPORT MaterialInterface {
public:
    explicit MaterialInterface(std::shared_ptr<ShaderInterface> shader)
        : shader_(std::move(shader)) {}

    /// @brief Binds the material (delegates to the shader).
    virtual void bind() = 0;

    /// @brief Optional: Unbind material (if backend requires it).
    virtual void unbind() = 0;

    /// @brief Gets the shader associated with this material.
    /// @return A shared pointer to the shader interface.
    std::shared_ptr<ShaderInterface> getShader() const { return shader_; }

private:
    std::shared_ptr<ShaderInterface> shader_;
};

using MaterialHandle = std::shared_ptr<MaterialInterface>;

} // namespace jelly::graphics