// TransformSystem.hpp
#pragma once

#include "jelly/jelly_export.hpp"

#include "transform.hpp"
#include "hierarchy.hpp"
#include "game_system_interface.hpp"

#include <entt/entt.hpp>

namespace jelly::core {

class JELLY_EXPORT TransformSystem final : public jelly::core::GameSystemInterface {
public:
    explicit TransformSystem(entt::registry& registry);

    void update() override;

private:
    entt::registry& registry_;
};

} // namespace jelly::core
