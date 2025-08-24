#pragma once

#include "vulkan_graphic_api.hpp"
#include "vulkan_shader_module.hpp"
#include "jelly/jelly_export.hpp"
#include "jelly/core/managed_resource.hpp"
#include "jelly/graphics/shader_interface.hpp"

#include <vulkan/vulkan.h>
#include <memory>
#include <array>

namespace jelly::graphics::vulkan {

/// @brief Packed shader uniforms (16-byte aligned)
struct alignas(16) ShaderUniforms {
    float model[16];      // Model transformation matrix
    float view[16];       // View matrix
    float projection[16]; // Projection matrix
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

    /// @brief Releases all GPU resources associated with this shader
    /// @note Must be called before destruction if the shader needs explicit cleanup
    void release() override;

    /// @brief Sets vec3 uniform value
    /// @param name Uniform name (must match shader)
    /// @param vec Pointer to 3 consecutive floats
    void setUniformVec3(const char* name, const float* vec) override;

    /// @brief Sets mat4 uniform value
    /// @param name Uniform name (must match shader) 
    /// @param matrix Pointer to 16 consecutive floats (column-major)
    void setUniformMat4(const char* name, const float* matrix) override;

    /// @brief Updates texture binding in descriptor set
    /// @param imageView Texture image view to bind
    /// @param sampler Texture sampler to bind
    /// @param binding Binding point in the shader
    /// @param frameIndex Frame index for double buffering (0 or 1)
    void updateTextureDescriptor(VkImageView imageView, VkSampler sampler, uint32_t binding, uint32_t frameIndex);

    /// @brief Gets vertex shader module
    const VulkanShaderModule* getVertexModule() const;
    
    /// @brief Gets fragment shader module
    const VulkanShaderModule* getFragmentModule() const;

    /// @brief Gets descriptor set layout
    VkDescriptorSetLayout getDescriptorSetLayout() const;
    
    /// @brief Gets descriptor set for specific frame index
    /// @param frameIndex Index of the frame (0 to MAX_FRAMES_IN_FLIGHT-1)
    /// @return Vulkan descriptor set handle
    VkDescriptorSet getDescriptorSet(uint32_t frameIndex) const; 

private:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    VulkanGraphicAPI* api_;
    std::unique_ptr<VulkanShaderModule> vertex_;
    std::unique_ptr<VulkanShaderModule> fragment_;

    ShaderUniforms uniforms_;

    // Managed Vulkan resources
    std::array<jelly::core::ManagedResource<VkBuffer>, MAX_FRAMES_IN_FLIGHT> uniformBuffers_;
    std::array<jelly::core::ManagedResource<VkDeviceMemory>, MAX_FRAMES_IN_FLIGHT> uniformBufferMemories_;
    jelly::core::ManagedResource<VkDescriptorSetLayout> descriptorSetLayout_;
    jelly::core::ManagedResource<VkDescriptorPool> descriptorPool_;
    std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> descriptorSets_{};

    /// @brief Creates uniform buffers
    void createUniformBuffers();
    
    /// @brief Creates descriptor set layout  
    void createDescriptorSetLayout();
    
    /// @brief Creates descriptor pool
    void createDescriptorPool();
    
    /// @brief Allocates descriptor sets
    void allocateDescriptorSets();
    
    /// @brief Updates descriptor sets bindings
    void updateDescriptorSets();
};

} // namespace jelly::graphics::vulkan