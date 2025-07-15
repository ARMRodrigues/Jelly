#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"
#include "jelly/core/logger.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace jelly::graphics::vulkan {

using jelly::Exception;
using jelly::core::Logger;
using jelly::core::LogLevel;

void VulkanGraphicAPI::createInstance() {
    std::vector<const char*> extensions = windowProvider_->getVulkanRequiredExtensions();

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Jelly";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Jelly Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;

    VkInstance rawInstance = VK_NULL_HANDLE;

    if (vkCreateInstance(&createInfo, nullptr, &rawInstance) != VK_SUCCESS) {
        throw Exception("Failed to create Vulkan instance!");
    }

    instance_ = ManagedResource<VkInstance>(
        rawInstance,
        [](VkInstance inst) { vkDestroyInstance(inst, nullptr); }
    );

    uint32_t apiVersion = 0;
    if (vkEnumerateInstanceVersion(&apiVersion) == VK_SUCCESS) {
        char message[128];
        snprintf(message, sizeof(message),
                "Vulkan instance created (API version %u.%u.%u)",
                VK_VERSION_MAJOR(apiVersion),
                VK_VERSION_MINOR(apiVersion),
                VK_VERSION_PATCH(apiVersion));
        Logger::Log(LogLevel::Info, message);
    }
}

}