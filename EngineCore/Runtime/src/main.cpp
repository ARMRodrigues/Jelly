#include "jelly/jelly.hpp"
#include "jelly/core/graphic_api_type.hpp"
#include "jelly/core/window_settings.hpp"

int main() {
    jelly::Jelly jelly;

    jelly::core::WindowSettings windowSettings;
    windowSettings.width = 1280;
    windowSettings.height = 720;
    windowSettings.title = "Jelly Engine";
    windowSettings.vsync = true;

    jelly::core::WindowSettings settings = {1280, 720, true, "JellyEngine"};

    bool result = jelly.initialize(jelly::core::GraphicAPIType::Vulkan, settings);

    while (jelly.isRunning())
    {
        jelly.pollEvents();
        jelly.render();
    }

    jelly.shutdown();
    
}