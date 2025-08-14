#include "jelly/core/camera_system.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace jelly::core {

CameraSystem::CameraSystem(entt::registry &registry, float viewportWidth, float viewportHeight)
    : registry_(registry), width_(viewportWidth), height_(viewportHeight)
{
}

void CameraSystem::update()
{
    auto view = registry_.view<Transform, Camera>();

    view.each([this](auto /*entity*/, Transform &transform, Camera &camera) {
        const glm::vec3 pos = glm::vec3(transform.worldMatrix[3]);
        const glm::vec3 fwd = -glm::vec3(transform.worldMatrix[2]); // forward (-Z)
        const glm::vec3 up  =  glm::vec3(transform.worldMatrix[1]); // up

        // VIEW
        camera.view = glm::lookAtRH(pos, pos + fwd, up);

        // PROJECTION
        camera.aspect = (height_ > 0.0f) ? (width_ / height_) : 1.0f;
        if (camera.type == CameraType::Perspective) {
            camera.projection = glm::perspectiveRH_ZO(
                glm::radians(camera.fieldOfViewDegrees),
                camera.aspect,
                camera.nearPlane,
                camera.farPlane
            );
        } else {
            float top =  camera.orthographicSize;
            float bottom = -camera.orthographicSize;
            float right =  camera.orthographicSize * camera.aspect;
            float left  = -camera.orthographicSize * camera.aspect;
            camera.projection = glm::orthoRH_ZO(left, right, bottom, top, camera.nearPlane, camera.farPlane);
        }
    });
}

void CameraSystem::onResize(float w, float h)
{
    width_ = w;
    height_ = h;
}

} // namespace jelly::core
