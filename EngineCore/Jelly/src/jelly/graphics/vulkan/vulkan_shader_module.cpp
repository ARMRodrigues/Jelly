#include "jelly/graphics/vulkan/vulkan_shader_module.hpp"
#include <stdexcept>

namespace jelly::graphics::vulkan {

VulkanShaderModule::VulkanShaderModule(VkDevice device, const std::vector<uint8_t>& spirvCode, VkShaderStageFlagBits stage)
    : device_(device), stage_(stage) {
    
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = spirvCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(spirvCode.data());

    if (vkCreateShaderModule(device_, &createInfo, nullptr, &module_) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan shader module.");
    }
}

VulkanShaderModule::~VulkanShaderModule() {
    if (module_) {
        vkDestroyShaderModule(device_, module_, nullptr);
    }
}

} // namespace jelly::graphics::vulkan
