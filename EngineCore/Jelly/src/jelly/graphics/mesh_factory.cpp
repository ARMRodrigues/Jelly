#include "jelly/graphics/mesh_factory.hpp"
#include <stdexcept>

namespace jelly::graphics {

std::vector<std::weak_ptr<Mesh>> MeshFactory::meshes_;
std::mutex MeshFactory::mutex_;

MeshHandle MeshFactory::create() {
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
