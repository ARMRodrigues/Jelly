#include "jelly/graphics/material_factory.hpp"

#include <stdexcept>

namespace jelly::graphics {

std::vector<std::weak_ptr<MaterialInterface>> MaterialFactory::materials_;
std::mutex MaterialFactory::mutex_;

MaterialHandle MaterialFactory::create(std::shared_ptr<ShaderInterface> shader) {
    switch (GraphicContext::get().getAPIType()) {
        case core::GraphicAPIType::Vulkan: {
            auto material = std::make_shared<vulkan::VulkanMaterial>(shader);
            auto api = static_cast<vulkan::VulkanGraphicAPI*>(GraphicContext::get().getAPI());
            material->createPipeline(api);
            registerMaterial(material);
            return material;
        }
        default:
            throw std::runtime_error("Unsupported graphics API in MaterialFactory");
    }
}

void MaterialFactory::releaseAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& weakMat : materials_) {
        if (auto mat = weakMat.lock()) {
            mat->release();
        }
    }
    materials_.clear();
}

void MaterialFactory::registerMaterial(const MaterialHandle& material) {
    std::lock_guard<std::mutex> lock(mutex_);
    materials_.push_back(material);
}

} // namespace jelly::graphics
