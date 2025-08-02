#include "jelly/graphics/shader_factory.hpp"

#include "jelly/graphics/graphic_context.hpp"
#include "jelly/graphics/vulkan/vulkan_shader.hpp"
#include "jelly/graphics/vulkan/vulkan_graphic_api.hpp"

namespace jelly::graphics {

std::shared_ptr<ShaderInterface> ShaderFactory::createFromFiles(const std::string& shaderPath) {

    auto& context = GraphicContext::get();
    auto api = context.getAPIType();

    if (api == core::GraphicAPIType::Vulkan) {
        auto* vkApi = static_cast<vulkan::VulkanGraphicAPI*>(context.getAPI());
        VkDevice device = vkApi->getDevice();

        auto vsPath = resolveShaderPath(shaderPath, "vertex", "vulkan");
        auto fsPath = resolveShaderPath(shaderPath, "fragment", "vulkan");

        std::vector<uint8_t> vsCode = readBinaryFile(vsPath);
        std::vector<uint8_t> fsCode = readBinaryFile(fsPath);

        auto vertex = std::make_unique<vulkan::VulkanShaderModule>(device, vsCode, VK_SHADER_STAGE_VERTEX_BIT);
        auto fragment = std::make_unique<vulkan::VulkanShaderModule>(device, fsCode, VK_SHADER_STAGE_FRAGMENT_BIT);

        return std::make_shared<vulkan::VulkanShader>(std::move(vertex), std::move(fragment));
    }

    return nullptr;
}

std::filesystem::path ShaderFactory::resolveShaderPath(
    const std::string& shaderName,
    const std::string& stage,     // "vertex" ou "fragment"
    const std::string& backend)   // "vulkan", "opengl", "directx", etc.
{
    namespace fs = std::filesystem;
    fs::path basePath = "shaders";

    // Extensão por backend
    std::string extension;
    if (backend == "vulkan") {
        extension = ".spv";
    } else if (backend == "opengl") {
        extension = (stage == "vertex") ? ".vert" : ".frag";
    } else if (backend == "directx") {
        extension = ".hlsl";
    } else if (backend == "metal") {
        extension = ".metal";
    } else {
        throw std::runtime_error("Unknown backend: " + backend);
    }

    return basePath / shaderName / backend / (stage + extension);
}

std::string ShaderFactory::readTextFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open shader source file: " + path.string());
    }

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

std::vector<uint8_t> ShaderFactory::readBinaryFile(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open shader file: " + path.string());
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<uint8_t> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    return buffer;
}

} // namespace jelly::graphics
