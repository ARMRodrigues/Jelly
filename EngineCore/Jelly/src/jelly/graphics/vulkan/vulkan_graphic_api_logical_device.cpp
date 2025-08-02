#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

#include "jelly/exception.hpp"

namespace jelly::graphics::vulkan {

void VulkanGraphicAPI::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice_, surface_);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueFamilies = {
        indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t family : uniqueFamilies) {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = family;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

    const char *extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensions;

    VkDevice rawDevice = VK_NULL_HANDLE;
    if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &rawDevice) != VK_SUCCESS) {
        throw jelly::Exception("Failed to create logical device!");
    }

    device_ = ManagedResource<VkDevice>(
        rawDevice,
        [](VkDevice dev) { vkDestroyDevice(dev, nullptr); },
        VK_NULL_HANDLE
    );

    vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
    vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);
}

}