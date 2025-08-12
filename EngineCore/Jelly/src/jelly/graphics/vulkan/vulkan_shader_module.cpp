#include "jelly/graphics/vulkan/vulkan_shader_module.hpp"
#include <stdexcept>
#include <iostream>
namespace jelly::graphics::vulkan {

VulkanShaderModule::VulkanShaderModule(
    VkDevice device,
    const std::vector<uint8_t>& spirvCode,
    VkShaderStageFlagBits stage
)
    : device_(device), stage_(stage)
{

    if (device == VK_NULL_HANDLE) {
        throw std::runtime_error("Invalid Vulkan device (VK_NULL_HANDLE) in VulkanShaderModule constructor");
    }

    if (spirvCode.empty()) {
        throw std::runtime_error("SPIR-V code vector is empty");
    }

    if (spirvCode.size() % 4 != 0) {
        throw std::runtime_error("SPIR-V code size must be multiple of 4");
    }
    
    VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    createInfo.codeSize = spirvCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(spirvCode.data());

    VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &module_);
    if (result != VK_SUCCESS) {
        std::cerr << "vkCreateShaderModule failed with error code: " << result << std::endl;
        throw std::runtime_error("Failed to create shader module");
    }
}

VulkanShaderModule::~VulkanShaderModule() {
    if (module_ != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device_, module_, nullptr);
    }
}

} // namespace jelly::graphics::vulkan
