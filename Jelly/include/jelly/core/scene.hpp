#pragma once

#include "jelly/core/game_system_interface.hpp"

#include <entt/entt.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace jelly::core {

/// @brief Represents a scene that holds entities and game systems.
/// 
/// A Scene manages an EnTT registry and a collection of game systems.
/// It provides a standard update lifecycle.
class Scene {
public:
    /// @brief Constructs a scene with the given name.
    /// @param name The name of the scene.
    explicit Scene(std::string name);

    /// @brief Gets the name of the scene.
    /// @return A constant reference to the scene name.
    const std::string& getName() const;

    /// @brief Accesses the EnTT entity registry.
    /// @return A reference to the scene's entity registry.
    entt::registry& getEntityManager();

    /// @brief Adds a game system to the scene.
    /// @param system A shared pointer to the system to add.
    void addGameSystem(std::shared_ptr<GameSystemInterface> system);

    /// @brief Calls `initialize()` on all game systems.
    void initialize();

    /// @brief Calls `update()` on all game systems.
    void update();

    /// @brief Calls `fixedUpdate()` on all game systems.
    void fixedUpdate();

    /// @brief Calls `render()` on all game systems.
    void render();

    /// @brief Calls `shutdown()` on all game systems.
    void shutdown();

private:
    std::string name_;
    entt::registry registry_;
    std::vector<std::shared_ptr<GameSystemInterface>> gameSystems_;
};

}
