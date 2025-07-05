#pragma once

namespace jelly::core {
/// <summary>
/// Struct containing window creation settings.
/// </summary>
struct WindowSettings {
    int width;           ///< Width of the window in pixels.
    int height;          ///< Height of the window in pixels.
    bool vsync;          ///< Whether VSync should be enabled.
    const char* title;   ///< Title of the window.
};

}
