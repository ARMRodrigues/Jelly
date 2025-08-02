#pragma once

#include "jelly/core/window_settings.hpp"

namespace jelly::windowing {

using jelly::core::WindowSettings;

/// <summary>
/// Interface for platform-specific window systems (e.g., GLFW, SDL).
/// </summary>
class WindowSystemInterface {
public:
    virtual ~WindowSystemInterface() = default;

    /// <summary>
    /// Creates the native window using the specified settings.
    /// </summary>
    virtual void createWindow(const WindowSettings& settings) = 0;

    /// <summary>
    /// Makes the window visible on screen. Useful to delay showing until setup is complete.
    /// </summary>
    virtual void showWindow() = 0;

    /// <summary>
    /// Returns true if the window is still open and not requested to close.
    /// </summary>
    virtual bool isWindowOpen() const = 0;

    /// <summary>
    /// Polls platform events (input, resize, close requests, etc.).
    /// </summary>
    virtual void pollEvents() = 0;

    /// <summary>
    /// Destroys the window and releases its resources.
    /// </summary>
    virtual void destroyWindow() = 0;
};

}
