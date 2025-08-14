#include "jelly/jelly.hpp"
#include "jelly/core/graphic_api_type.hpp"
#include "jelly/core/window_settings.hpp"
#include "jelly/graphics/shader_factory.hpp"
#include "jelly/graphics/material.hpp"
#include "jelly/graphics/mesh_factory.hpp"
#include "jelly/graphics/mesh_renderer_system.hpp"
#include "jelly/graphics/material_factory.hpp"
#include "jelly/core/transform.hpp"
#include "jelly/core/hierarchy.hpp"
#include "jelly/core/transform_system.hpp"
#include "jelly/core/camera.hpp"
#include "jelly/core/camera_system.hpp"

struct Vertex {
    float position[3];
};

int main() {
    jelly::Jelly jelly;

    jelly::core::WindowSettings windowSettings;
    windowSettings.width = 1280;
    windowSettings.height = 720;
    windowSettings.title = "Jelly Engine";
    windowSettings.vsync = true;

    jelly::core::WindowSettings settings = {1280, 720, true, "JellyEngine"};

    bool result = jelly.initialize(jelly::core::GraphicAPIType::Vulkan, settings);

    // TODO : remove to another project later
    auto scene = std::make_unique<jelly::core::Scene>("MyScene");

    auto& registry = scene->getEntityManager();

    // Transform system
    auto transformSystem = std::make_shared<jelly::core::TransformSystem>(scene->getEntityManager());
    scene->addGameSystem(transformSystem);

    // Creating camera
    auto cameraEntity = scene->getEntityManager().create();
    auto& cameraTransform = registry.emplace<jelly::core::Transform>(cameraEntity);
    cameraTransform.setLocalPosition(glm::vec3(0.0f, 0.0f, 10.0f));

    jelly::core::Camera cam;
    cam.type = jelly::core::CameraType::Perspective;
    cam.fieldOfViewDegrees = 60.0f;
    cam.nearPlane = 0.03f;
    cam.farPlane = 1000.0f;
    registry.emplace<jelly::core::Camera>(cameraEntity, cam);

    auto cameraSystem = std::make_shared<jelly::core::CameraSystem>(registry, (float)1280, (float)720);
    scene->addGameSystem(cameraSystem);

    // Mesh Entity
    auto entity = scene->getEntityManager().create();
    registry.emplace<jelly::core::Transform>(entity);

    auto shader = jelly::graphics::ShaderFactory::createFromFiles("triangle");

    auto material = jelly::graphics::MaterialFactory::create(shader);

    Vertex triangleVertices[] = {
    {{ 0.0f,  0.5f, 0.0f }},
    {{ 0.5f, -0.5f, 0.0f }},
    {{-0.5f, -0.5f, 0.0f }},
    };

    uint32_t triangleIndices[] = { 0, 1, 2 };

    auto mesh = jelly::graphics::MeshFactory::create();
    mesh->upload(triangleVertices, sizeof(triangleVertices), triangleIndices, sizeof(triangleIndices));

    registry.emplace<jelly::graphics::MeshComponent>(entity, mesh);
    registry.emplace<jelly::graphics::MaterialComponent>(entity, material);

    auto meshRendererSystem = std::make_shared<jelly::graphics::MeshRendererSystem>(scene->getEntityManager());
    scene->addGameSystem(meshRendererSystem);

    auto sceneId = jelly.getSceneManager().addScene(std::move(scene));
    jelly.getSceneManager().setActiveScene(0);

    while (jelly.isRunning())
    {
        jelly.pollEvents();
        jelly.update();
        jelly.render();
    }

    jelly.shutdown();
    
}