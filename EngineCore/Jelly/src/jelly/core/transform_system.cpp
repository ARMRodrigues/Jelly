#include "jelly/core/transform_system.hpp"

namespace jelly::core {

TransformSystem::TransformSystem(entt::registry& registry)
    : registry_(registry)
{
}

void TransformSystem::update() {
        // 1. Entidades sem pai → worldMatrix = localMatrix
        auto noParentView = registry_.view<Transform>(entt::exclude<Hierarchy>);
        for (auto e : noParentView) {
            auto& t = noParentView.get<Transform>(e);
            if (t.hasTransformValuesChanged) {
                t.rebuildLocalMatrix();
                t.hasTransformValuesChanged = false;
            }
            t.worldMatrix = t.localMatrix;
        }

        // 2. Entidades com pai → herda matriz do pai
        auto parentView = registry_.view<Transform, Hierarchy>();
        parentView.each([&](auto entity, Transform &t, Hierarchy &h) {
            if (t.hasTransformValuesChanged) {
                t.rebuildLocalMatrix();
                t.hasTransformValuesChanged = false;
            }

            for (auto child : h.children) {
                if (auto* childTransform = registry_.try_get<Transform>(child)) {
                    if (childTransform->hasTransformValuesChanged) {
                        childTransform->rebuildLocalMatrix();
                        childTransform->hasTransformValuesChanged = false;
                    }
                    childTransform->worldMatrix = h.parent != entt::null
                        ? childTransform->localMatrix * t.worldMatrix
                        : childTransform->localMatrix;
                }
            }
        });
    }


} // namespace jelly::core
