#pragma once

#include "jelly/jelly.hpp"

#include <glm/glm.hpp>

namespace jelly::core {

/// @brief Defines the projection type of a camera.
enum class CameraType { 
    Perspective, // Perspective projection (3D view with depth perception).
    Orthographic // Orthographic projection (parallel projection, no perspective).
};

/// @brief Represents a camera with projection and view matrices.
/// 
/// A camera can be either perspective or orthographic, storing
/// its projection parameters and the matrices used for rendering.
struct JELLY_EXPORT Camera {
    CameraType type{CameraType::Perspective}; /// Camera projection type.
    float fieldOfViewDegrees {60.0f}; // Vertical field of view in degrees (perspective mode).
    float nearPlane{0.03f}; // Distance to the near clipping plane.
    float farPlane{1000.0f}; // Distance to the far clipping plane.
    float orthographicSize {5.0f}; // Half of the orthographic view height (orthographic mode).
    float aspect{16.0f / 9.0f}; // Aspect ratio (width / height).

    glm::mat4 view{1.0f}; // View matrix (world-to-camera transformation).
    glm::mat4 projection{1.0f}; // Projection matrix (camera-to-clip-space transformation).
};

} // namespace jelly::core
