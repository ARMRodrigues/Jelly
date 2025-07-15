#pragma once

#include "native_window_handle_provider_interface.hpp"

#include <vulkan/vulkan.h>
#include <vector>

namespace jelly::windowing {

/// <summary>
/// Vulkan-specific window handle operations.
/// </summary>
class VulkanNativeWindowHandleProvider : public NativeWindowHandleProviderInterface {
public:
    virtual ~VulkanNativeWindowHandleProvider() = default;

    /// Retorna as extensões obrigatórias para criar a surface Vulkan.
    virtual std::vector<const char*> getVulkanRequiredExtensions() = 0;

    /// Cria a Vulkan surface associada à janela.
    virtual VkSurfaceKHR createVulkanSurface(VkInstance instance) = 0;
};

}
