#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace jelly::windowing {

/// Interface for platform-specific window handles, abstracted from any graphics API.
class NativeWindowHandleProviderInterface {
public:
    virtual ~NativeWindowHandleProviderInterface() = default;

    /// Returns a pointer to the native window handle (e.g., HWND, GLFWwindow*, NSWindow*).
    virtual void* getNativeWindowHandle() = 0;

    /// Retrieves the framebuffer size in pixels (width × height).
    virtual void getFramebufferSize(uint32_t& width, uint32_t& height) = 0;

    /// Waits for window system events (e.g., input, resize).
    virtual void waitEvents() = 0;
};

}
