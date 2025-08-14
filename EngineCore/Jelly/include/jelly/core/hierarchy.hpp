// Hierarchy.hpp
#pragma once

#include "jelly/jelly_export.hpp"

#include <entt/entt.hpp>

#include <vector>
#include <cstdint>

namespace jelly::core {

struct JELLY_EXPORT Hierarchy {
    entt::entity parent{entt::null};
    std::vector<entt::entity> children;
};

} // namespace jelly::core
