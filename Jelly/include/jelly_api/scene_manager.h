#pragma once

#include "jelly_api_export.h"

JELLY_API_BEGIN

typedef struct JellyHandle JellyHandle;

/// @brief Creates a new scene and adds it to the SceneManager.
/// @param engine A pointer to the Jelly engine handle.
/// @param name The name of the new scene.
/// @return The ID of the created scene, or -1 on failure.
JELLY_API int jelly_scene_manager_create_scene(JellyHandle* engine, const char* name);

/// @brief Sets the active scene by its ID.
/// @param engine A pointer to the Jelly engine handle.
/// @param sceneId The ID of the scene to activate.
/// @return 1 if successful, or -1 if the scene does not exist.
JELLY_API int jelly_scene_manager_set_active_scene(JellyHandle* engine, int sceneId);

/// @brief Destroys a scene and releases its resources.
/// @param engine A pointer to the Jelly engine handle.
/// @param sceneId The ID of the scene to destroy.
/// @return 1 if the scene was successfully removed, or 0 if not found.
JELLY_API int jelly_scene_manager_destroy_scene(JellyHandle* engine, int sceneId);

JELLY_API_END
