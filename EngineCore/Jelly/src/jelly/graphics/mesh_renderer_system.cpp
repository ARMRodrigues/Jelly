#include "jelly/graphics/mesh_renderer_system.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace jelly::graphics {

MeshRendererSystem::MeshRendererSystem(entt::registry& registry)
    : registry_(registry) {}

void MeshRendererSystem::render() {
    auto view = registry_.view<MeshComponent, MaterialComponent, core::Transform>();
    auto camView = registry_.view<core::Camera, core::Transform>();

    // Find first active camera
    for (auto [entity, camera, transform] : camView.each()) {
        glm::mat4 viewMatrix = camera.view;
        glm::mat4 projectionMatrix = camera.projection;

        view.each([&](auto entity, MeshComponent& mesh, MaterialComponent& material, 
                     core::Transform& transform) {
            auto shader = material.material->getShader();
            shader->setUniformMat4("model", glm::value_ptr(transform.worldMatrix));
            shader->setUniformMat4("view", glm::value_ptr(viewMatrix));
            shader->setUniformMat4("projection", glm::value_ptr(projectionMatrix));

            material.material->bind();
            mesh.mesh->draw();
        });
        break; // Only use first camera found
    }
}

} // namespace jelly::graphics