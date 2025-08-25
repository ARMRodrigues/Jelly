#pragma once

#include "mesh.hpp"
#include "graphic_context.hpp"

#include "vulkan/vulkan_mesh.hpp"
#include "vulkan/vulkan_graphic_api.hpp"

#include "jelly/jelly_export.hpp"

#include "jelly/core/graphic_api_type.hpp"

#include <stdexcept>
#include <memory>
#include <mutex>
#include <vector>

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
    static MeshHandle createMeshHandle();

    /// @brief Creates a quad mesh (two triangles forming a rectangle)
    /// @return A MeshHandle to a quad mesh with predefined vertex data
    static MeshHandle quad();

    /// @brief Creates a cube mesh
    /// @return A MeshHandle to a cube mesh with predefined vertex data
    static MeshHandle cube();

    /// @brief Releases all cached mesh resources
    /// @note Must be called before graphics device destruction
    static void releaseAll();

private:
    /// @brief Registers a mesh instance for tracking and management
    /// @param mesh The mesh handle to register
    static void registerMesh(const MeshHandle& mesh);

    static std::vector<std::weak_ptr<Mesh>> meshes_;
    static std::mutex mutex_;
};

} // namespace jelly::graphics