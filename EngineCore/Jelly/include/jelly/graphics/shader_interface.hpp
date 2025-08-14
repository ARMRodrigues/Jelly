#pragma once

#include "jelly/jelly_export.hpp"

namespace jelly::graphics {

/// @brief Abstract base class for GPU shader programs. Implemented per backend (OpenGL, Vulkan, etc.).
class JELLY_EXPORT ShaderInterface {
public:
    virtual ~ShaderInterface() = default;

    /// @brief Bind the shader (if the backend requires it).
    virtual void bind() = 0;

    /// @brief Unbind the shader (optional).
    virtual void unbind() = 0;

    /// @brief Sets a 3-component vector uniform
    /// @param name Uniform variable name in shader
    /// @param vec Pointer to 3 consecutive float values
    virtual void setUniformVec3(const char* name, const float* vec) {}

    /// @brief Sets a 4x4 matrix uniform
    /// @param name Uniform variable name in shader
    /// @param matrix Pointer to 16 consecutive float values (column-major)
    virtual void setUniformMat4(const char* name, const float* matrix) {}
};

} // namespace jelly::graphics
