#include "jelly_api/jelly.h"

#include "jelly/jelly.hpp"
#include "jelly/core/graphics_api_type.hpp"
#include "jelly/core/window_settings.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace {

using jelly::core::GraphicsAPIType;

// -----------------------------------------------------------------------------
// Converts a string to lowercase and returns the corresponding GraphicsAPIType enum.
// Throws std::invalid_argument if the API is not recognized.
// -----------------------------------------------------------------------------
GraphicsAPIType ParseGraphicsAPIType(const char *name) {
    std::string lower{name};
    std::transform(
        lower.begin(), lower.end(), lower.begin(),
        [](unsigned char c)
        { return std::tolower(c);
    });

    if (lower == "noapi") {
        return GraphicsAPIType::NoApi;
    }
    if (lower == "opengl") {
        return GraphicsAPIType::OpenGL;
    }
    if (lower == "vulkan") {
        return GraphicsAPIType::Vulkan;
    }

    throw std::runtime_error("Unknown Graphics API: " + lower);
}

}

struct JellyHandle final {
    jelly::Jelly instance;
};

JELLY_API JellyHandle* jelly_create() {
    return new JellyHandle();
}

JELLY_API int jelly_initialize(JellyHandle* engineHandle, int width, int height, bool vsync, const char *title, const char *apiName) {
    if (!engineHandle) return 0;

    jelly::core::WindowSettings settings = {width, height, vsync, title};

    GraphicsAPIType apiNameEnum;
    try
    {
        apiNameEnum = ParseGraphicsAPIType(apiName);
    }
    catch (const std::exception& e)
    {
        std::cerr << "[jelly_initialize] Error: " << e.what() << '\n';
        return 0;
    }

    return engineHandle->instance.initialize(static_cast<jelly::core::GraphicsAPIType>(apiNameEnum), settings);
}

JELLY_API int jelly_is_running(JellyHandle* engine) {
    if (!engine) return 0;
    return engine->instance.isRunning() ? 1 : 0;
}

JELLY_API void jelly_poll_events(JellyHandle* engine) {
    if (!engine) return;
    engine->instance.pollEvents();
}

JELLY_API void jelly_render(JellyHandle* engine) {
    if (!engine) return;
    engine->instance.render();
}

JELLY_API void jelly_destroy(JellyHandle* engine) {
    if (!engine) return;
    engine->instance.shutdown();
    delete engine;
}