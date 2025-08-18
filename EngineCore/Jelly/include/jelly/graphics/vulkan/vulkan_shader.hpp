#pragma once

#include "vulkan_graphic_api.hpp"
#include "vulkan_shader_module.hpp"

#include "jelly/jelly_export.hpp"

#include "jelly/core/managed_resource.hpp"

#include "jelly/graphics/shader_interface.hpp"

#include <vulkan/vulkan.h>

#include <memory>

namespace jelly::graphics::vulkan {

/// @brief Packed shader uniforms (16-byte aligned)
struct alignas(16) ShaderUniforms {
    float model[16];      ///< Model transformation matrix
    float view[16];       ///< View matrix
    float projection[16]; ///< Projection matrix
};

/// @brief Vulkan implementation of ShaderInterface
class JELLY_EXPORT VulkanShader : public graphics::ShaderInterface {
public:
    /// @brief Creates shader from vertex/fragment modules
    VulkanShader(
        VulkanGraphicAPI* api,
        std::unique_ptr<VulkanShaderModule> vertex,
        std::unique_ptr<VulkanShaderModule> fragment);

    /// @brief Binds the shader for rendering
    void bind() override;
    
    /// @brief Unbinds the shader (no-op in Vulkan)
    void unbind() override;

    /// @brief Sets vec3 uniform value
    /// @param name Uniform name (must match shader)
    /// @param vec Pointer to 3 consecutive floats
    void setUniformVec3(const char* name, const float* vec) override;

    /// @brief Sets mat4 uniform value
    /// @param name Uniform name (must match shader) 
    /// @param matrix Pointer to 16 consecutive floats (column-major)
    void setUniformMat4(const char* name, const float* matrix) override;

    /// @brief Gets vertex shader module
    const VulkanShaderModule* getVertexModule() const;
    
    /// @brief Gets fragment shader module
    const VulkanShaderModule* getFragmentModule() const;

    /// @brief Gets descriptor set layout
    VkDescriptorSetLayout getDescriptorSetLayout() const;
    
    /// @brief Gets descriptor set
    VkDescriptorSet getDescriptorSet() const;

private:
    VulkanGraphicAPI* api_;
    std::unique_ptr<VulkanShaderModule> vertex_;
    std::unique_ptr<VulkanShaderModule> fragment_;

    ShaderUniforms uniforms_;

    // Managed Vulkan resources
    jelly::core::ManagedResource<VkBuffer> uniformBuffer_;
    jelly::core::ManagedResource<VkDeviceMemory> uniformBufferMemory_;
    jelly::core::ManagedResource<VkDescriptorSetLayout> descriptorSetLayout_;
    jelly::core::ManagedResource<VkDescriptorPool> descriptorPool_;
    VkDescriptorSet descriptorSet_ = VK_NULL_HANDLE;

    /// @brief Creates uniform buffer
    void createUniformBuffer();
    
    /// @brief Creates descriptor set layout  
    void createDescriptorSetLayout();
    
    /// @brief Creates descriptor pool
    void createDescriptorPool();
    
    /// @brief Allocates descriptor set
    void allocateDescriptorSet();
    
    /// @brief Updates descriptor set bindings
    void updateDescriptorSet();
};

} // namespace jelly::graphics::vulkan