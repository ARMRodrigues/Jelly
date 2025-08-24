#pragma once

#include "vulkan_graphic_api.hpp"
#include "vulkan_shader_module.hpp"

#include "jelly/jelly_export.hpp"
#include "jelly/core/managed_resource.hpp"
#include "jelly/graphics/shader_interface.hpp"

#include "spirv-reflect/spirv_reflect.h"

#include <vulkan/vulkan.h>

#include <memory>
#include <array>
#include <unordered_map>
#include <vector>
#include <string>

namespace jelly::graphics::vulkan {

struct TextureBinding {
    std::string name;
    uint32_t set;
    uint32_t binding;
    VkSampler sampler = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
};

/// @brief Vulkan implementation of ShaderInterface com uniforms genéricos
class JELLY_EXPORT VulkanShader : public graphics::ShaderInterface {
public:
    /// @brief Creates shader from vertex/fragment modules
    VulkanShader(
        VulkanGraphicAPI* api,
        std::unique_ptr<VulkanShaderModule> vertex,
        std::unique_ptr<VulkanShaderModule> fragment
    );

    /// @brief Binds the shader for rendering
    void bind() override;

    /// @brief Unbinds the shader (no-op in Vulkan)
    void unbind() override;
    
    /// @brief Releases all GPU resources associated with this shader
    /// @note Must be called before destruction if the shader needs explicit cleanup
    void release() override;

    // Uniforms
    
    /// @brief Sets vec3 uniform value
    /// @param name Uniform name (must match shader)
    /// @param vec Pointer to 3 consecutive floats
    void setUniformVec3(const char* name, const float* vec) override;
    
    /// @brief Sets mat4 uniform value
    /// @param name Uniform name (must match shader) 
    /// @param matrix Pointer to 16 consecutive floats (column-major)
    void setUniformMat4(const char* name, const float* matrix) override;

    // Texturess / Descriptors

    /// @brief Updates texture binding in descriptor set
    /// @param imageView Texture image view to bind
    /// @param sampler Texture sampler to bind
    /// @param binding Binding point in the shader
    /// @param frameIndex Frame index for double buffering (0 or 1)
    void updateTextureDescriptor(VkImageView imageView, VkSampler sampler, uint32_t binding, uint32_t frameIndex);

    /// @brief Gets descriptor set layout
    VkDescriptorSetLayout getDescriptorSetLayout() const;

    /// @brief Gets descriptor set for specific frame index
    /// @param frameIndex Index of the frame (0 to MAX_FRAMES_IN_FLIGHT-1)
    /// @return Vulkan descriptor set handle
    VkDescriptorSet getDescriptorSet(uint32_t frameIndex) const;

    /// @brief Gets the binding point for a uniform by name
    /// @param name Name of the uniform variable in the shader
    /// @return Binding point index, or UINT32_MAX if not found
    uint32_t getUniformBinding(const std::string& name) const;

    /// @brief Gets the sampler binding point for a texture by name
    /// @param name Name of the texture/sampler in the shader
    /// @return Binding point index, or UINT32_MAX if not found
    uint32_t getTextureBinding(const std::string& name) const;

    /// @brief Gets vertex shader module
    const VulkanShaderModule* getVertexModule() const;

    /// @brief Gets fragment shader module
    const VulkanShaderModule* getFragmentModule() const;

private:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    VulkanGraphicAPI* api_;
    std::unique_ptr<VulkanShaderModule> vertex_;
    std::unique_ptr<VulkanShaderModule> fragment_;

    std::vector<uint8_t> cpuUniformData;
    size_t uniformBufferSize = 0;
    std::unordered_map<std::string, size_t> uniformOffsets;

    std::unordered_map<std::string, uint32_t> samplerBindings;
    std::unordered_map<std::string, uint32_t> textureNameToBinding;
    std::unordered_map<uint32_t, TextureBinding> boundTextures;

    std::array<jelly::core::ManagedResource<VkBuffer>, MAX_FRAMES_IN_FLIGHT> uniformBuffers_;
    std::array<jelly::core::ManagedResource<VkDeviceMemory>, MAX_FRAMES_IN_FLIGHT> uniformBufferMemories_;
    jelly::core::ManagedResource<VkDescriptorSetLayout> descriptorSetLayout_;
    jelly::core::ManagedResource<VkDescriptorPool> descriptorPool_;
    std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> descriptorSets_{};

    jelly::core::ManagedResource<VkImage> defaultTextureImage_;
    jelly::core::ManagedResource<VkDeviceMemory> defaultTextureMemory_;
    jelly::core::ManagedResource<VkImageView> defaultTextureView_;
    jelly::core::ManagedResource<VkSampler> defaultTextureSampler_;

    /// @brief Analyzes SPIR-V code to extract uniform and binding information
    void reflectUniforms();

    /// @brief Reflects a specific shader module's SPIR-V code
    /// @param code SPIR-V bytecode to analyze
    void reflectShaderCode(const std::vector<uint8_t>& code);

    /// @brief Creates uniform buffers
    void createUniformBuffers();

    /// @brief Creates descriptor set layout  
    void createDescriptorSetLayout();

    /// @brief Creates descriptor pool
    void createDescriptorPool();

    /// @brief Allocates descriptor sets
    void allocateDescriptorSets();

    /// @brief Creates and initializes a default fallback texture
    void initializeDefaultTexture();

    /// @brief Updates descriptor sets bindings
    void updateDescriptorSets();

    /// @brief Flushes CPU uniform data to GPU buffer for a specific frame
    /// @param frameIndex Frame index to update (0 or 1)
    void flushUniforms(uint32_t frameIndex);
};

} // namespace jelly::graphics::vulkan
