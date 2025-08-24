#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace jelly::graphics {

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

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
    virtual void upload() = 0;

    /// @brief Sets the vertex positions for the mesh
    /// @param position Vector of 3D position coordinates
    void setPositions(const std::vector<glm::vec3>& position) { positions_ = position; }

    /// @brief Sets the primary UV coordinates for the mesh
    /// @param uv0 Vector of 2D texture coordinates
    void setUV0(const std::vector<glm::vec2>& uv0) { uv0_ = uv0; }

    /// @brief Sets the index data for the mesh
    /// @param indices Vector of vertex indices defining triangles
    void setIndices(const std::vector<uint32_t>& indices) { indices_ = indices; }

    /// @brief Binds the mesh's buffers and issues a draw command.
    ///
    /// This function is called during the rendering loop to draw the mesh.
    virtual void draw() const = 0;

    /// @brief Releases all GPU resources associated with this shader
    /// @note Must be called before destruction if the shader needs explicit cleanup
    virtual void release() = 0;

protected:
    std::vector<glm::vec3> positions_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec4> tangents_;
    std::vector<glm::vec4> colors_;
    std::vector<glm::vec2> uv0_;
    std::vector<glm::vec2> uv1_;
    std::vector<uint32_t> indices_;

    /// @brief Builds an interleaved vertex buffer from separate attribute arrays
    /// @return Vector of interleaved Vertex structures ready for GPU upload
    std::vector<Vertex> buildVertexBuffer() const;
};

/// @brief A shared pointer to a Mesh object, used for managing its lifecycle.
using MeshHandle = std::shared_ptr<Mesh>;

} // namespace jelly::graphics