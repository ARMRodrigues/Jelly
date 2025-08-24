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
