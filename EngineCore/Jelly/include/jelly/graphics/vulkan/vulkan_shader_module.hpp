#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace jelly::graphics::vulkan {

/// @brief Represents a compiled Vulkan shader module for a specific stage.
///
/// This class encapsulates the creation and destruction of a `VkShaderModule`,
/// and stores the shader stage it is intended for (e.g., vertex, fragment).
class VulkanShaderModule {
public:
    /// @brief Constructs a VulkanShaderModule from SPIR-V bytecode.
    ///
    /// @param device The Vulkan logical device used to create the shader module.
    /// @param spirvCode The SPIR-V bytecode of the shader.
    /// @param stage The Vulkan shader stage flag (e.g., VK_SHADER_STAGE_VERTEX_BIT).
    VulkanShaderModule(VkDevice device, const std::vector<uint8_t>& spirvCode, VkShaderStageFlagBits stage);

    /// @brief Destroys the Vulkan shader module.
    ~VulkanShaderModule();

    /// @brief Returns the Vulkan shader module handle.
    /// @return VkShaderModule handle.
    VkShaderModule getModule() const { return module_; }

    /// @brief Returns the shader stage this module is intended for.
    /// @return VkShaderStageFlagBits indicating the shader stage.
    VkShaderStageFlagBits getStage() const { return stage_; }

    /// @brief Returns the SPIR-V bytecode used to create this shader module
    /// @return Reference to the SPIR-V bytecode vector
    const std::vector<uint8_t>& getSPIRVCode() const { return spirvCode_; }

private:
    VkDevice device_;                   // Logical device used for module creation.
    VkShaderModule module_;             // Vulkan shader module handle.
    VkShaderStageFlagBits stage_;       // Shader stage associated with this module.
    std::vector<uint8_t> spirvCode_;    // SPIR-V bytecode vector
};

} // namespace jelly::graphics::vulkan
