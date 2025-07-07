#pragma once

#include "jelly/core/managed_resource.hpp"
#include "jelly/windowing/window_system_interface.hpp"

struct GLFWwindow;

namespace jelly::windowing::glfw {

using jelly::core::ManagedResource;

class GLFWWindowSystem : public WindowSystemInterface {
public:
    GLFWWindowSystem() = default;
    virtual ~GLFWWindowSystem();
    
    GLFWWindowSystem(const GLFWWindowSystem&) = delete;
    GLFWWindowSystem& operator=(const GLFWWindowSystem&) = delete;

    GLFWWindowSystem(GLFWWindowSystem&&) noexcept = default;
    GLFWWindowSystem& operator=(GLFWWindowSystem&&) noexcept = default;

    void showWindow() override;
    bool isWindowOpen() const override;
    void pollEvents() override;
    void destroyWindow() override;

protected:
    ManagedResource<GLFWwindow*> window_;

    virtual void createWindow(const WindowSettings& settings) override;
};

}