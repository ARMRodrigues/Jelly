#pragma once

#include "jelly/jelly.hpp"

/// Internal definition of opaque JellyHandle for C bindings.
struct JellyHandle final {
    jelly::Jelly instance;
};