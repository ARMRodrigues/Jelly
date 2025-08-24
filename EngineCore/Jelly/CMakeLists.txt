project(Jelly LANGUAGES CXX)

# Dependencies

# Vulkan
find_package(Vulkan REQUIRED)

# FetchContent for GLFW and EnTT
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# EnTT
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.15.0
)
FetchContent_MakeAvailable(entt)

# glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# Source/header directories

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jelly)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/jelly)

# Header files (optional, used for IDE organization and install)
set(HEADERS
    ${HEADER_DIR}/jelly.hpp
    ${HEADER_DIR}/error.hpp
    ${HEADER_DIR}/exception.hpp
    ${HEADER_DIR}/core/logger.hpp
    ${HEADER_DIR}/core/managed_resource.hpp
    ${HEADER_DIR}/core/window_settings.hpp
    ${HEADER_DIR}/core/graphic_api_type.hpp
    ${HEADER_DIR}/core/game_system_interface.hpp
    ${HEADER_DIR}/core/scene.hpp
    ${HEADER_DIR}/core/scene_manager.hpp
    ${HEADER_DIR}/core/transform.hpp
    ${HEADER_DIR}/core/hierarchy.hpp
    ${HEADER_DIR}/core/camera.hpp
    ${HEADER_DIR}/core/transform_system.hpp
    ${HEADER_DIR}/core/camera_system.hpp
    ${HEADER_DIR}/graphics/graphic_api_interface.hpp
    ${HEADER_DIR}/graphics/graphic_api_factory.hpp
    ${HEADER_DIR}/graphics/graphic_context.hpp
    ${HEADER_DIR}/graphics/shader_interface.hpp
    ${HEADER_DIR}/graphics/shader_factory.hpp
    ${HEADER_DIR}/graphics/material.hpp
    ${HEADER_DIR}/graphics/mesh.hpp
    ${HEADER_DIR}/graphics/mesh_factory.hpp
    ${HEADER_DIR}/graphics/mesh_renderer_system.hpp
    ${HEADER_DIR}/graphics/material_factory.hpp
    ${HEADER_DIR}/graphics/image.hpp
    ${HEADER_DIR}/graphics/texture_interface.hpp
    ${HEADER_DIR}/graphics/texture_factory.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_graphic_api.hpp
    ${HEADER_DIR}/graphics/vulkan/queue_family_indices.hpp
    ${HEADER_DIR}/graphics/vulkan/swap_chain_support_details.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_shader_module.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_shader.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_buffer_utils.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_mesh.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_material.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_texture.hpp
    ${HEADER_DIR}/windowing/window_system_interface.hpp
    ${HEADER_DIR}/windowing/native_window_handle_provider_interface.hpp
    ${HEADER_DIR}/windowing/vulkan_native_window_handle_provider.hpp
    ${HEADER_DIR}/windowing/window_system_factory.hpp
    ${HEADER_DIR}/windowing/window_graphic_api_binder.hpp
    ${HEADER_DIR}/windowing/glfw/glfw_window_system.hpp
    ${HEADER_DIR}/windowing/glfw/glfw_vulkan_window_system.hpp
)

# Source files
set(SRC_FILES
    ${SRC_DIR}/../spirv-reflect/spirv_reflect.cpp
    ${SRC_DIR}/jelly.cpp
    ${SRC_DIR}/core/logger.cpp
    ${SRC_DIR}/core/scene.cpp
    ${SRC_DIR}/core/scene_manager.cpp
    ${SRC_DIR}/core/transform_system.cpp
    ${SRC_DIR}/core/camera_system.cpp
    ${SRC_DIR}/graphics/graphic_context.cpp
    ${SRC_DIR}/graphics/mesh_factory.cpp
    ${SRC_DIR}/graphics/shader_factory.cpp
    ${SRC_DIR}/graphics/material_factory.cpp
    ${SRC_DIR}/graphics/texture_factory.cpp
    ${SRC_DIR}/graphics/mesh_renderer_system.cpp
    ${SRC_DIR}/graphics/image.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_instance.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_surface.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_pick_physical_device.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_logical_device.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_swapchain.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_image_views.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_render_pass.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_framebuffers.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_command_pool.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_command_buffers.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_sync_objects.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_graphic_api_helpers.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_shader_module.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_shader.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_buffer_utils.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_mesh.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_material.cpp
    ${SRC_DIR}/graphics/vulkan/vulkan_texture.cpp
    ${SRC_DIR}/windowing/glfw/glfw_window_system.cpp
    ${SRC_DIR}/windowing/glfw/glfw_vulkan_window_system.cpp
)

# Library target
add_library(Jelly SHARED ${SRC_FILES} ${HEADERS})

# Compile Definitions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(JELLY_DEBUG)
endif()

# Windows: remove 'lib' prefix when using MinGW
if(WIN32 AND MINGW)
    set_target_properties(Jelly PROPERTIES
        ARCHIVE_OUTPUT_NAME "Jelly"
        PREFIX ""
    )
endif()

# Output directories
set_target_properties(Jelly PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

# Preprocessor definitions
target_compile_definitions(Jelly PRIVATE JELLY_BUILD_DLL)
target_compile_definitions(Jelly PUBLIC SPIRV_REFLECT_USE_SYSTEM_SPIRV_H)

# Include directories
target_include_directories(Jelly PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
    ${Vulkan_INCLUDE_DIRS}
)

# Linked libraries
target_link_libraries(Jelly PUBLIC
    glfw
    Vulkan::Vulkan
    EnTT
)
