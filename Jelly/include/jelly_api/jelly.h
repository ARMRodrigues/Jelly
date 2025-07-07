#pragma once

#include "jelly_api_export.h"

JELLY_API_BEGIN

// Forward declaration of opaque engine struct
typedef struct JellyHandle JellyHandle;

/// Creates a new Jelly engine instance.
/// @return Pointer to JellyHandle, or nullptr on failure.
JELLY_API JellyHandle* jelly_create();

/// Initializes the engine.
/// Returns 1 on success, 0 on failure.
JELLY_API int jelly_initialize(JellyHandle* engine, int width, int height, bool vsync, const char *title, const char *apiName);

/// Checks if the engine is still running (window open).
JELLY_API int jelly_is_running(JellyHandle* engine);

/// Polls events (input, window, etc.).
JELLY_API void jelly_poll_events(JellyHandle* engine);

/// Renders one frame.
JELLY_API void jelly_render(JellyHandle* engine);

/// Shuts down and destroys the engine.
JELLY_API void jelly_destroy(JellyHandle* engine);

JELLY_API_END
