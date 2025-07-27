#pragma once

namespace jelly::graphics {

/// @brief Abstract base class for GPU shader programs. Implemented per backend (OpenGL, Vulkan, etc.).
class ShaderInterface {
public:
    virtual ~ShaderInterface() = default;

    /// @brief Bind the shader (if the backend requires it).
    virtual void bind() = 0;

    /// @brief Unbind the shader (optional).
    virtual void unbind() = 0;
};

} // namespace jelly::graphics
