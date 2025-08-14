#include "jelly/core/transform_system.hpp"

namespace jelly::core {

TransformSystem::TransformSystem(entt::registry& registry)
    : registry_(registry)
{
}

void TransformSystem::update() {
    auto view = registry_.view<Transform>();
    for (auto e : view) {
        auto& t = view.get<Transform>(e);
        if (t.hasTransformValuesChanged) {
            t.rebuildLocalMatrix();
            t.hasTransformValuesChanged = false;
        }
    }
    
    auto rootView = registry_.view<Transform>(entt::exclude<Hierarchy>);
    for (auto e : rootView) {
        propagateWorldMatrix(e, glm::mat4(1.0f));
    }
}

void TransformSystem::propagateWorldMatrix(entt::entity entity, const glm::mat4& parentWorld) {
    auto& t = registry_.get<Transform>(entity);
    t.worldMatrix = parentWorld * t.localMatrix;

    if (auto* h = registry_.try_get<Hierarchy>(entity)) {
        for (auto child : h->children) {
            propagateWorldMatrix(child, t.worldMatrix);
        }
    }
}


} // namespace jelly::core
