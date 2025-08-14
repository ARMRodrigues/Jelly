#pragma once

#include "mesh.hpp"
#include "material.hpp"

#include "jelly/jelly_export.hpp"
#include "jelly/core/camera.hpp"
#include "jelly/core/transform.hpp"
#include "jelly/core/game_system_interface.hpp"

#include <entt/entt.hpp>

namespace jelly::graphics {

/// @brief Component that holds a renderable mesh
struct MeshComponent {
    MeshHandle mesh;
};

/// @brief Component that defines rendering appearance
struct MaterialComponent {
    std::shared_ptr<MaterialInterface> material;
};

/// @brief Renders entities with mesh and material components
class JELLY_EXPORT MeshRendererSystem : public core::GameSystemInterface {
public:
    explicit MeshRendererSystem(entt::registry& registry);

    /// @brief Renders all visible entities that have a mesh and material.
    void render() override;

private:
    entt::registry& registry_;
};

} // namespace jelly::graphics