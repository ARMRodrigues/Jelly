#pragma once

#include "jelly/core/scene.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace jelly::core {

class SceneManager {
public:
    /// @brief Adds a new scene to the manager.
    /// @param scene A unique pointer to the scene.
    /// @return The ID of the added scene.
    int addScene(std::unique_ptr<Scene> scene);

    /// @brief Gets a scene by its ID.
    /// @param id The ID of the scene.
    /// @return A shared pointer to the scene, or nullptr if not found.
    std::shared_ptr<Scene> getSceneById(int id);

    /// @brief Sets the active scene by its ID.
    /// @param sceneId The ID of the scene to activate.
    /// @return True if the scene was successfully set, false otherwise.
    bool setActiveScene(int sceneId);

    /// @brief Removes a scene by its ID.
    /// @param id The ID of the scene to remove.
    /// @return True if the scene was successfully removed.
    bool destroyScene(int id);

    /// @brief Initializes the active scene.
    void initializeActiveScene();

    /// @brief Updates the active scene.
    void updateActiveScene();

    /// @brief Performs a fixed update on the active scene.
    void fixedUpdate();

    /// @brief Renders the active scene.
    void renderActiveScene();

    /// @brief Shuts down the active scene.
    void shutdownActiveScene();

private:
    std::unordered_map<int, std::shared_ptr<Scene>> scenes_;
    std::unordered_map<std::string, int> nameToId_;
    std::shared_ptr<Scene> currentScene_;
    int currentSceneId_ = -1;
    int nextId_ = 0;
};

}
