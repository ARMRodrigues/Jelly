#pragma once
#include <memory>

namespace jelly::graphics {

/// @brief An interface for a renderable geometric mesh.
///
/// Defines the abstract base class for a mesh, which consists of vertex and
/// index data. Concrete implementations (e.g., for Vulkan or OpenGL) will
/// handle the GPU-specific buffer management and draw calls.
class Mesh {
public:
    /// @brief Default virtual destructor to allow for proper cleanup of derived classes.
    virtual ~Mesh() = default;

    /// @brief Uploads vertex and index data to the GPU.
    ///
    /// This function is responsible for creating and populating the GPU buffers
    /// with the geometric data that defines the mesh.
    /// @param vertexData A pointer to the raw vertex data.
    /// @param vertexSize The total size of the vertex data in bytes.
    /// @param indexData A pointer to the raw index data.
    /// @param indexSize The total size of the index data in bytes.
    virtual void upload(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize) = 0;

    /// @brief Binds the mesh's buffers and issues a draw command.
    ///
    /// This function is called during the rendering loop to draw the mesh.
    virtual void draw() const = 0;
};

/// @brief A shared pointer to a Mesh object, used for managing its lifecycle.
using MeshHandle = std::shared_ptr<Mesh>;

} // namespace jelly::graphics