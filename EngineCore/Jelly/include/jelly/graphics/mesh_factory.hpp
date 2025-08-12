#pragma once

#include "jelly/jelly_export.hpp"

#include "jelly/core/graphic_api_type.hpp"

#include "jelly/graphics/mesh.hpp"
#include "jelly/graphics/graphic_context.hpp"
#include "jelly/graphics/vulkan/vulkan_mesh.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include <stdexcept> // Required for std::runtime_error

namespace jelly::graphics {

/// @brief A factory for creating API-specific mesh instances.
///
/// This class abstracts the creation of `Mesh` objects. It queries the
/// `GraphicContext` to determine the active rendering backend and instantiates
/// the appropriate concrete `Mesh` implementation (e.g., `VulkanMesh`).
class JELLY_EXPORT MeshFactory {
public:
    /// @brief Creates a new, empty mesh object for the current graphics API.
    ///
    /// The created mesh is initially empty. Vertex and index data must be
    /// uploaded to it using the `Mesh::upload()` method before it can be drawn.
    /// @return A `MeshHandle` to the newly created, API-specific mesh.
    /// @throws std::runtime_error if the graphics API from the context is unsupported.
    static MeshHandle create() {
        auto& context = GraphicContext::get();
        switch (context.getAPIType()) {
            case core::GraphicAPIType::Vulkan: {
                auto api = static_cast<vulkan::VulkanGraphicAPI*>(context.getAPI());
                return std::make_shared<vulkan::VulkanMesh>(
                    api->getDevice(), api->getPhysicalDevice());
            }
            default:
                // This will be thrown if the API is something other than Vulkan
                throw std::runtime_error("Unsupported graphics API for MeshFactory");
        }
    }
};

} // namespace jelly::graphics