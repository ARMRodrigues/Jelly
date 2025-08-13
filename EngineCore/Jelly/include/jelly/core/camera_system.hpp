#pragma once

#include "camera.hpp"
#include "transform.hpp"
#include "game_system_interface.hpp"

#include "jelly/jelly_export.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace jelly::core {

class JELLY_EXPORT CameraSystem final : public jelly::core::GameSystemInterface
{
public:
    explicit CameraSystem(entt::registry &registry, float viewportWidth, float viewportHeight);

    void update() override;
    void onResize(float w, float h);

private:
    entt::registry &registry_;
    float width_{1280.0f};
    float height_{720.0f};
};

} // namespace jelly::core
