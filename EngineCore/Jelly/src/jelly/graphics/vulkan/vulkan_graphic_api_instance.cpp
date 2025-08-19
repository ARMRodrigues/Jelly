#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"
#include "jelly/core/logger.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string.h>

namespace jelly::graphics::vulkan {

using jelly::Exception;
using jelly::core::Logger;
using jelly::core::LogLevel;

bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) return false;
    }
    return true;
}

void VulkanGraphicAPI::createInstance() {

    std::vector<const char*> extensions = windowProvider_->getVulkanRequiredExtensions();

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Jelly";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Jelly Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    uint32_t apiVersion = appInfo.apiVersion;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef JELLY_DEBUG
    if (!checkValidationLayerSupport(validationLayers)) {
            throw Exception("Validation layers requested, but not available!");
        }
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

#ifdef JELLY_DEBUG
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
#elif
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.pNext = nullptr;
#endif

    VkInstance rawInstance = VK_NULL_HANDLE;
    if (vkCreateInstance(&createInfo, nullptr, &rawInstance) != VK_SUCCESS) {
        throw Exception("Failed to create Vulkan instance!");
    }

    instance_ = ManagedResource<VkInstance>(
        rawInstance,
        [](VkInstance inst) { vkDestroyInstance(inst, nullptr); }
    );

#ifdef JELLY_DEBUG
    setupDebugMessenger();
#endif
    char message[128];
    snprintf(message, sizeof(message),
        "Vulkan instance created (Requested API version %u.%u.%u)",
        VK_VERSION_MAJOR(apiVersion),
        VK_VERSION_MINOR(apiVersion),
        VK_VERSION_PATCH(apiVersion)
    );

    Logger::Log(LogLevel::Info, message);
}

#ifdef JELLY_DEBUG

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT              messageType,
    const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData,
    void*                                        pUserData) 
{
    Logger::Log(LogLevel::Debug, pCallbackData->pMessage);
    return VK_FALSE;
}

void VulkanGraphicAPI::populateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void VulkanGraphicAPI::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugCreateInfo(createInfo);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");

    if (func && func(instance_, &createInfo, nullptr, &debugMessenger_) != VK_SUCCESS) {
        throw Exception("Failed to create Debug Utils Messenger!");
    }
}

void VulkanGraphicAPI::destroyDebugMessenger() {
    if (debugMessenger_) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
        if (func) func(instance_, debugMessenger_, nullptr);
        debugMessenger_ = VK_NULL_HANDLE;
    }
}

#endif

} // namespace jelly::graphics::vulkan
