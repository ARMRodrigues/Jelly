#pragma once

namespace jelly::core {

/// @brief Interface for game systems used within a Scene.
/// 
/// Game systems implement behavior such as logic updates, rendering,
/// and physics. This interface defines the common lifecycle methods.
class GameSystemInterface {
public:
    /// @brief Virtual destructor.
    virtual ~GameSystemInterface() = default;

    /// @brief Called once when the system is initialized.
    virtual void initialize() {}

    /// @brief Called every frame to update the system.
    virtual void update() {}

    /// @brief Called at a fixed timestep for deterministic updates (e.g., physics).
    virtual void fixedUpdate() {}

    /// @brief Called every frame to perform rendering operations.
    virtual void render() {}

    /// @brief Called once to clean up system resources.
    virtual void shutdown() {}
};

} // namespace jelly::core
