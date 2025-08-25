#include "jelly/graphics/mesh_factory.hpp"

#include <stdexcept>

namespace jelly::graphics {

std::vector<std::weak_ptr<Mesh>> MeshFactory::meshes_;
std::mutex MeshFactory::mutex_;

MeshHandle MeshFactory::createMeshHandle() {
    auto& context = GraphicContext::get();
    switch (context.getAPIType()) {
        case core::GraphicAPIType::Vulkan: {
            auto api = static_cast<vulkan::VulkanGraphicAPI*>(context.getAPI());
            auto mesh = std::make_shared<vulkan::VulkanMesh>(
                api->getDevice(), api->getPhysicalDevice());
            registerMesh(mesh);
            return mesh;
        }
        default:
            throw std::runtime_error("Unsupported graphics API for MeshFactory");
    }
}

MeshHandle MeshFactory::quad()
{
    auto mesh = createMeshHandle();

    mesh->setPositions({
        {-0.5f, -0.5f, 0.0f}, // bottom-left
        { 0.5f, -0.5f, 0.0f}, // bottom-right
        { 0.5f,  0.5f, 0.0f}, // top-right
        {-0.5f,  0.5f, 0.0f}  // top-left
    });

    mesh->setUV0({
        {0.0f, 0.0f}, // bottom-left
        {1.0f, 0.0f}, // bottom-right
        {1.0f, 1.0f}, // top-right
        {0.0f, 1.0f}  // top-left
    });

    mesh->setIndices({
        0, 1, 2,
        2, 3, 0
    });

    mesh->upload();

    return mesh;
}

MeshHandle MeshFactory::cube()
{
    auto mesh = createMeshHandle();

    std::vector<glm::vec3> positions = {
        // Front face
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
        // Back face
        { 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
        // Left face
        {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
        // Right face
        { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
        // Top face
        {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
        // Bottom face
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}
    };

    mesh->setPositions(positions);

    std::vector<glm::vec2> uv0;
    for(int i=0;i<6;i++){
        uv0.push_back({0,0});
        uv0.push_back({1,0});
        uv0.push_back({1,1});
        uv0.push_back({0,1});
    }

    mesh->setUV0(uv0);

    mesh->setIndices({
        // Front face
        0, 1, 2,  0, 2, 3,
        // Back face
        4, 5, 6,  4, 6, 7,
        // Left face
        8, 9, 10, 8, 10, 11,
        // Right face
        12, 13, 14, 12, 14, 15,
        // Top face
        16, 17, 18, 16, 18, 19,
        // Bottom face
        20, 21, 22, 20, 22, 23
    });

    mesh->upload();
    return mesh;
}

void MeshFactory::releaseAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& weakMesh : meshes_) {
        if (auto mesh = weakMesh.lock()) {
            mesh->release();
        }
    }
    meshes_.clear();
}

void MeshFactory::registerMesh(const MeshHandle& mesh) {
    std::lock_guard<std::mutex> lock(mutex_);
    meshes_.push_back(mesh);
}

} // namespace jelly::graphics
