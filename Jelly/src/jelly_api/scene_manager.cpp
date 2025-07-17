#include "jelly_api/scene_manager.h"

#include "jelly_api/jelly_handle.h"

int jelly_scene_manager_create_scene(JellyHandle* engine, const char* name) {
    if (!engine || !name) return -1;

    auto* jelly = reinterpret_cast<jelly::Jelly*>(engine);
    auto& manager = jelly->getSceneManager();

    std::string sceneName(name);
    auto scene = std::make_unique<jelly::core::Scene>(sceneName);

    auto resultId = manager.addScene(std::move(scene));

    return resultId >= 0 ? resultId : -1;
}

int jelly_scene_manager_set_active_scene(JellyHandle* engine, int sceneId) {
    if (!engine) return 0;

    auto* jelly = reinterpret_cast<jelly::Jelly*>(engine);
    auto& manager = jelly->getSceneManager();

    bool result = manager.setActiveScene(sceneId);
    return result ? 1 : 0;
}

int jelly_scene_manager_destroy_scene(JellyHandle* engine, int sceneId) {
    if (!engine) return 0;

    auto* jelly = reinterpret_cast<jelly::Jelly*>(engine);
    auto& manager = jelly->getSceneManager();

    bool success = manager.destroyScene(sceneId);
    return success ? 1 : 0;
}

