#pragma once

#include "jelly/core/graphics_api_type.hpp"
#include "jelly/core/window_settings.hpp"
#include "jelly/windowing/window_system_interface.hpp"

#include <memory>

namespace jelly {

using jelly::core::GraphicsAPIType;
using jelly::core::WindowSettings;
using jelly::windowing::WindowSystemInterface;

/// <summary>
/// Core engine class responsible for initializing the window and graphics system.
/// </summary>
class Jelly {
public:
    Jelly() = default;
    ~Jelly() = default;

    // Delete copy and move semantics to enforce single instance behavior.
    Jelly(const Jelly&) = delete;
    Jelly& operator=(const Jelly&) = delete;
    Jelly(Jelly&&) = delete;
    Jelly& operator=(Jelly&&) = delete;

    /// <summary>
    /// Initializes the engine with the selected graphics API and window settings.
    /// </summary>
    /// <param name="graphicsAPIType">The graphics API to use (e.g., Vulkan).</param>
    /// <param name="windowSettings">Window configuration options.</param>
    /// <returns>True if initialization succeeded.</returns>
    bool initialize(GraphicsAPIType graphicsAPIType, const WindowSettings& windowSettings);

    /// <summary>
    /// Returns true if the engine is running in headless mode (no graphics or window).
    /// </summary>
    bool isHeadless() const;

    /// <summary>
    /// Returns true if the engine should keep running (i.e., window is still open).
    /// </summary>
    bool isRunning() const;

    /// <summary>
    /// Polls input and window events (e.g., keyboard, resize, close).
    /// </summary>
    void pollEvents();

    /// <summary>
    /// Renders a single frame (calls beginFrame/endFrame internally).
    /// </summary>
    void render();

    /// <summary>
    /// Shuts down the engine and releases all resources.
    /// </summary>
    void shutdown();

private:
    bool isHeadless_ = false;
    std::unique_ptr<WindowSystemInterface> windowSystem_;
};

}
