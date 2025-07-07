#include "jelly/jelly.hpp"

#include "jelly/windowing/window_system_factory.hpp"

namespace jelly {

    bool Jelly::initialize(GraphicsAPIType graphicsAPIType, const WindowSettings &windowSettings) {
        
        // Headless mode
        if (graphicsAPIType == GraphicsAPIType::NoApi) {
            isHeadless_ = true;
            return true;
        }

        windowSystem_ = jelly::windowing::WindowSystemFactory::create(graphicsAPIType);

        if (!windowSystem_) {
            return false;
        }

        windowSystem_->createWindow(windowSettings);

        return true;
    }

    bool Jelly::isHeadless() const {
        return isHeadless_; 
    }

    bool Jelly::isRunning() const {
        if (isHeadless_) return true;
        return windowSystem_ && windowSystem_->isWindowOpen();
    }

    void Jelly::pollEvents() {
        if (!isHeadless_ && windowSystem_) {
            windowSystem_->pollEvents();
        }
    }

    void Jelly::render() {
        if (!isHeadless_ && windowSystem_) {
        }
    }

    void Jelly::shutdown() {
        if (!isHeadless_ && windowSystem_) {
            windowSystem_->destroyWindow();
            windowSystem_.reset();
        }
    }
}
