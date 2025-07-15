#pragma once

#include "jelly/windowing/window_system_interface.hpp"

namespace jelly::graphics {

class GraphicAPIInterface {
public:
    virtual ~GraphicAPIInterface() = default;

    /// Initializes the graphics API (e.g., create device, setup context).
    virtual void initialize() {}

    /// Initializes the graphics API (e.g., create device, setup context).
    //virtual void initialize(jelly::windowing::WindowSystemInterface* windowSystem) { initialize(); }

    /// Begins rendering a new frame.
    virtual void beginFrame() = 0;

    /// Ends rendering of the current frame.
    virtual void endFrame() = 0;

    /// Releases all resources and shuts down the API.
    virtual void shutdown() = 0;
};

}