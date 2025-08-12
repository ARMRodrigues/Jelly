#pragma once

#include "jelly/core/game_system_interface.hpp"

#include "jelly/graphics/mesh.hpp"
#include "jelly/graphics/material.hpp"

#include <memory>

#include <entt/entt.hpp>

namespace jelly::graphics {

/// @brief A component that attaches a renderable mesh to an entity.
struct MeshComponent {
    /// @brief A handle to the `Mesh` object containing vertex and index data.
    MeshHandle mesh;
};

/// @brief A component that attaches a material to an entity for rendering.
struct MaterialComponent {
    /// @brief A handle to the `MaterialInterface` object, which defines the appearance (shader, textures).
    std::shared_ptr<MaterialInterface> material;
};

/// @brief A system responsible for rendering all entities that have a mesh and a material.
/// @details This system, part of the Entity-Component-System (ECS) architecture,
/// iterates through entities with both `MeshComponent` and `MaterialComponent`
/// and issues the appropriate draw calls for them each frame.
class MeshRendererSystem : public core::GameSystemInterface {
public:
    /// @brief Constructs the MeshRendererSystem.
    /// @param registry A reference to the EnTT registry that manages entities and components.
    explicit MeshRendererSystem(entt::registry& registry)
        : registry_(registry) {}

    /// @brief Renders all visible entities that have a mesh and material.
    /// @details Queries the registry for entities with both `MeshComponent` and
    /// `MaterialComponent`. For each entity found, it binds the material and
    /// then calls the mesh's draw method.
    void render() override {
        auto view = registry_.view<MeshComponent, MaterialComponent>();
        view.each([](auto entity, MeshComponent& mesh, MaterialComponent& material) {
            // Bind the material first to set up the shader and its parameters.
            material.material->bind();
            // Then, issue the draw call for the mesh.
            mesh.mesh->draw();
        });
    }

private:
    /// @brief A reference to the application's central entity-component registry.
    entt::registry& registry_;
};

} // namespace jelly::graphics