#pragma once

#include "transform.hpp"
#include "hierarchy.hpp"
#include "game_system_interface.hpp"

#include "jelly/jelly_export.hpp"

#include <entt/entt.hpp>

namespace jelly::core {


/// @brief System responsible for managing entity transforms and their hierarchy.
///
/// Handles local-to-world space transformations and parent-child relationships.
class JELLY_EXPORT TransformSystem final : public jelly::core::GameSystemInterface {
public:
    /// @brief Constructs a TransformSystem for the given entity registry.
    /// 
    /// @param registry Reference to the ECS registry that contains transform entities.
    explicit TransformSystem(entt::registry& registry);

    
    /// @brief Updates all transforms in the hierarchy, recalculating world matrices.
    void update() override;

private:
    entt::registry& registry_;

    /// @brief Recursively propagates a parent's world matrix to its children.
    ///
    /// @param entity The child entity to update.
    /// @param parentWorld The world matrix of the parent entity.
    void propagateWorldMatrix(entt::entity entity, const glm::mat4& parentWorld);
};

} // namespace jelly::core