#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/graphics/shader_interface.hpp"
#include "jelly/core/graphic_api_type.hpp"

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace jelly::graphics {

/// @brief Factory class for creating shader objects based on the current graphics backend.
///
/// This utility abstracts shader loading logic and backend-specific file resolution.
/// It supports both text (e.g., GLSL) and binary (e.g., SPIR-V) shader formats.
class JELLY_EXPORT ShaderFactory {
public:
    /// @brief Creates a shader instance for the current graphics API backend.
    ///
    /// This method will resolve the appropriate shader file(s) based on the selected backend
    /// (e.g., Vulkan, OpenGL) and construct a corresponding ShaderInterface implementation.
    ///
    /// @param vertexPath The base path or name of the vertex shader (without backend or stage suffix).
    /// @return A shared pointer to a ShaderInterface implementation.
    static std::shared_ptr<ShaderInterface> createFromFiles(const std::string& vertexPath);

private:
    /// @brief Resolves the complete shader file path based on name, stage, and backend.
    ///
    /// For example, calling with ("basic", "frag", "vulkan") might return "shaders/basic.frag.vulkan.spv".
    ///
    /// @param shaderName Base name or logical name of the shader.
    /// @param stage Shader stage (e.g., "vert", "frag").
    /// @param backend Backend name (e.g., "vulkan", "opengl").
    /// @return The full filesystem path to the shader file.
    static std::filesystem::path resolveShaderPath(
        const std::string& shaderName,
        const std::string& stage,
        const std::string& backend);

    /// @brief Reads a text file into a string.
    ///
    /// Typically used for loading GLSL shader source code.
    ///
    /// @param path Path to the file to read.
    /// @return Contents of the file as a std::string.
    static std::string readTextFile(const std::filesystem::path& path);

    /// @brief Reads a binary file into a byte buffer.
    ///
    /// Typically used for loading SPIR-V shader binaries.
    ///
    /// @param path Path to the file to read.
    /// @return Contents of the file as a std::vector<uint8_t>.
    static std::vector<uint8_t> readBinaryFile(const std::filesystem::path& path);
};

} // namespace jelly::graphics
