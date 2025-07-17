#include "jelly/jelly.hpp"

#include "jelly/exception.hpp"
#include "jelly/windowing/window_system_factory.hpp"
#include "jelly/graphics/graphic_api_factory.hpp"
#include "jelly/windowing/window_graphic_api_binder.hpp"

namespace jelly {

bool Jelly::initialize(GraphicAPIType graphicAPIType, const WindowSettings &windowSettings) {
    
    sceneManager_ = std::make_unique<core::SceneManager>();

    // Headless mode
    if (graphicAPIType == GraphicAPIType::NoApi) {
        isHeadless_ = true;
        return true;
    }

    windowSystem_ = windowing::WindowSystemFactory::create(graphicAPIType);

    if (!windowSystem_) {
        return false;
    }

    windowSystem_->createWindow(windowSettings);

    graphicAPI_ = graphics::GraphicsAPIFactory::create(graphicAPIType);

    if (!graphicAPI_) {
        return false;
    }

    if (!windowing::WindowGraphicAPIBinder::bind(graphicAPIType, windowSystem_.get(), graphicAPI_.get())) {
        return false;
    }
    
    graphicAPI_->initialize();

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
        graphicAPI_->beginFrame();
        sceneManager_->renderActiveScene();
        graphicAPI_->endFrame();
    }
}

void Jelly::shutdown() {
    if (!isHeadless_ && windowSystem_) {
        graphicAPI_->shutdown();
        windowSystem_->destroyWindow();
        windowSystem_.reset();
    }
}

SceneManager& Jelly::getSceneManager() {
    if (!sceneManager_) {
        throw Exception("SceneManager has not been initialized.");
    }
    return *sceneManager_;
}

}
