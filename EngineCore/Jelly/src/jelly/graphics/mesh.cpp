#include "jelly/graphics/mesh.hpp"

namespace jelly::graphics {

std::vector<Vertex> Mesh::buildVertexBuffer() const {
    std::vector<Vertex> vertices;
    vertices.reserve(positions_.size());

    for (size_t i = 0; i < positions_.size(); i++) {
        Vertex v{};
        v.pos = positions_[i];
        v.uv  = (i < uv0_.size()) ? uv0_[i] : glm::vec2(0.0f);
        vertices.push_back(v);
    }
    return vertices;
}

}