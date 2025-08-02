#pragma once

#include <cstdint>
#include <optional>

namespace jelly::graphics::vulkan {

/// Stores indices of queue families that support required Vulkan operations.
struct QueueFamilyIndices {
    /// Index of a queue family that supports graphics commands.
    std::optional<std::uint32_t> graphicsFamily;

    /// Index of a queue family that supports presentation to a surface.
    std::optional<std::uint32_t> presentFamily;

    /// Returns true if both graphics and presentation queue families are found.
    [[nodiscard]] bool IsComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

}
