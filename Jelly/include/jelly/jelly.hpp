#pragma once

#include "jelly/core/graphic_api_type.hpp"
#include "jelly/core/window_settings.hpp"
#include "jelly/core/scene_manager.hpp"
#include "jelly/graphics/graphic_api_interface.hpp"
#include "jelly/windowing/window_system_interface.hpp"

#include <memory>

namespace jelly {

using core::GraphicAPIType;
using core::WindowSettings;
using core::SceneManager;
using graphics::GraphicAPIInterface;
using windowing::WindowSystemInterface;

/**
 * @brief Core engine class responsible for initializing the window and graphics system.
 */
class Jelly {
public:
    Jelly() = default;
    ~Jelly() = default;

    // Delete copy and move semantics to enforce single instance behavior.
    Jelly(const Jelly&) = delete;
    Jelly& operator=(const Jelly&) = delete;
    Jelly(Jelly&&) = delete;
    Jelly& operator=(Jelly&&) = delete; 

    /**
    * @brief Initializes the engine with the selected graphics API and window settings.
    * 
    * @param graphicsAPIType The graphics API to use (e.g., Vulkan).
    * @param windowSettings Window configuration options.
    * @return True if initialization succeeded.
    */
    bool initialize(GraphicAPIType graphicAPIType, const WindowSettings& windowSettings);

    /**
    * @brief Returns true if the engine is running in headless mode (no graphics or window).
    */
    bool isHeadless() const;

    /**
    * @brief Returns true if the engine should keep running (i.e., window is still open).
    */
    bool isRunning() const;

    /**
    * @brief Polls input and window events (e.g., keyboard, resize, close).
    */
    void pollEvents();

    /**
    * @brief Renders a single frame (calls beginFrame/endFrame internally).
    */
    void render();

    /**
    * @brief Shuts down the engine and releases all resources
    */
    void shutdown();

    // Getters
    SceneManager& getSceneManager();

private:
    bool isHeadless_ = false;
    
    std::unique_ptr<WindowSystemInterface> windowSystem_;
    std::unique_ptr<GraphicAPIInterface> graphicAPI_;
    std::unique_ptr<SceneManager> sceneManager_;
};

}
