#pragma once

#include "jelly/core/camera.hpp"
#include "jelly/core/game_time.hpp"
#include "jelly/core/transform.hpp"
#include "jelly/core/game_system_interface.hpp"

#include <entt/entt.hpp>

#include <iostream>

struct Rotate {
    float speed;
};

class RotateMeshSystem : public jelly::core::GameSystemInterface {
public:
    explicit RotateMeshSystem(entt::registry& registry)
        : registry_(registry) {}

    void update() override {
        float dt = jelly::core::GameTime::deltaTime();

        auto view = registry_.view<jelly::core::Transform, Rotate>();

        view.each([dt](auto& transform, auto& rotate){
            glm::vec3 eulerDelta = glm::vec3(rotate.speed * dt);

            glm::quat incremental = glm::quat(eulerDelta);

            glm::quat newRotation = incremental * transform.localRotation;

            transform.setLocalRotationQuat(newRotation);
        });
    }

private:
    entt::registry& registry_;
};