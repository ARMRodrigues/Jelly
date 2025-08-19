#pragma once

#include "material.hpp"
#include "graphic_context.hpp"
#include "shader_interface.hpp"

#include "jelly/jelly_export.hpp"

#include "jelly/core/graphic_api_type.hpp"

#include "jelly/graphics/vulkan/vulkan_material.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>

namespace jelly::graphics {

/// @brief Creates and manages graphics API-specific material instances
class JELLY_EXPORT MaterialFactory {
public:
    /// @brief Creates a new material instance configured with a specific shader.
    ///
    /// @param shader A shared pointer to the shader program that the material will use.
    /// @return A `MaterialHandle` to the newly created, API-specific material.
    /// @throws std::runtime_error if the graphics API reported by the `GraphicContext` is not supported.
    static MaterialHandle create(std::shared_ptr<ShaderInterface> shader);

    /// @brief Releases all cached materials (call before device destruction)
    static void releaseAll();

private:
    /// @brief Registers a material instance in the factory's tracking system
    /// @param material The material handle to be registered and tracked
    static void registerMaterial(const MaterialHandle& material);

    static std::vector<std::weak_ptr<MaterialInterface>> materials_;
    static std::mutex mutex_;
};

} // namespace jelly::graphics