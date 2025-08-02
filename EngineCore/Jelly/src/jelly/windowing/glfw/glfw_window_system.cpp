#include "jelly/windowing/glfw/glfw_window_system.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace jelly::windowing::glfw {

GLFWWindowSystem::~GLFWWindowSystem() {
    destroyWindow();
}

void GLFWWindowSystem::createWindow(const WindowSettings& settings) {
    if (window_) {
        throw std::runtime_error("Window already created");
    }

    auto* rawWindow = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);

    if (!rawWindow) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    window_ = ManagedResource<GLFWwindow*>(
        rawWindow,
        [](GLFWwindow* w) {
            if (w) glfwDestroyWindow(w);
        },
        nullptr
    );
}

void GLFWWindowSystem::showWindow() {
    auto* w = window_.get();
    if (w) glfwShowWindow(w);
}

bool GLFWWindowSystem::isWindowOpen() const {
    auto* w = window_.get();
    return w && !glfwWindowShouldClose(w);
}

void GLFWWindowSystem::pollEvents() {
    glfwPollEvents();

    // TODO : remove, only to debug before having vulkan swapchain
    glfwSwapBuffers(window_.get());
}

void GLFWWindowSystem::destroyWindow() {
    window_.reset();
}

}