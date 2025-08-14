#pragma once

#include "jelly/jelly_export.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace jelly::core {
/// @brief Represents position, rotation, and scale in 3D space.
/// 
/// This component stores local transformation data and provides helper methods
/// for manipulating and retrieving direction vectors, matrices, and orientation.
struct JELLY_EXPORT Transform {
    // --- Local space ---
    glm::vec3 localPosition{0.0f}; // Local position relative to parent.
    glm::quat localRotation{1,0,0,0}; // Local rotation (w, x, y, z).
    glm::vec3 localScale{1.0f}; // Local scale relative to parent.

    // --- Cached data ---
    glm::mat4 localMatrix{1.0f}; // Local transformation matrix (T * R * S).
    glm::mat4 worldMatrix{1.0f}; // World transformation matrix.
    bool hasTransformValuesChanged {true}; // Marks if transformation needs to be recalculated.

    // --- Setters ---

    /// @brief Sets the local position.
    /// @param p New local position.
    void setLocalPosition(const glm::vec3& p) { localPosition = p; hasTransformValuesChanged = true; }

    /// @brief Sets the local rotation using Euler angles (in degrees).
    /// @param eulerDeg Euler angles in degrees.
    void setLocalRotationEuler(const glm::vec3& eulerDeg) {
        glm::vec3 radians = glm::radians(eulerDeg);
        localRotation = glm::quat(radians);
        hasTransformValuesChanged = true;
    }

    /// @brief Sets the local rotation using a quaternion.
    /// @param newQuaternion Quaternion representing the rotation.
    void setLocalRotationQuat(const glm::quat& newQuaternion) {
        localRotation = newQuaternion; 
        hasTransformValuesChanged = true; 
    }

    /// @brief Sets the local scale.
    /// @param newScale New local scale.
    void setLocalScale(const glm::vec3& newScale) {
        localScale = newScale; 
        hasTransformValuesChanged = true;
    }

    // --- Matrix operations ---

    /// @brief Rebuilds the local transformation matrix using T * R * S order.
    void rebuildLocalMatrix() {
        localMatrix = glm::translate(glm::mat4(1.0f), localPosition)
                    * glm::mat4_cast(localRotation)
                    * glm::scale(glm::mat4(1.0f), localScale);
    }

    // --- Direction vectors (world space) ---

    /// @brief Gets the right direction vector in world space.
    glm::vec3 right()   const { return glm::normalize(glm::vec3(worldMatrix[0])); }

    /// @brief Gets the up direction vector in world space.
    glm::vec3 up()      const { return glm::normalize(glm::vec3(worldMatrix[1])); }

    /// @brief Gets the forward direction vector in world space (-Z by default).
    glm::vec3 forward() const { return glm::normalize(-glm::vec3(worldMatrix[2])); }

    /// @brief Gets the world position extracted from the world matrix.
    glm::vec3 position() const { return glm::vec3(worldMatrix[3]); }

    // --- Orientation ---

    /// @brief Rotates the transform to look at a target position.
    /// @param target Target position in world space.
    /// @param worldUp Up direction in world space.
    void lookAt(const glm::vec3& target, const glm::vec3& worldUp) {
        glm::vec3 pos = position();
        glm::mat4 view = glm::lookAtRH(pos, target, worldUp);
        glm::mat4 worldFromView = glm::inverse(view);
        glm::quat q = glm::quat_cast(worldFromView);
        setLocalRotationQuat(q); // Works if no parent transform
    }
};

} // namespace jelly::core
