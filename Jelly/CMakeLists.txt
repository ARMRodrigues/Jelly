cmake_minimum_required (VERSION 3.16)

project(Jelly CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DOTNET_SDK "net8.0")
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../Output/${CMAKE_BUILD_TYPE}/${DOTNET_SDK}")

# ----------------------------------------------------------------------
# Vulkan
# ----------------------------------------------------------------------
find_package(Vulkan REQUIRED)

# ----------------------------------------------------------------------
# FetchContent
# ----------------------------------------------------------------------
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# entt
FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG        v3.15.0
)
FetchContent_MakeAvailable(entt)

# ----------------------------------------------------------------------
# Library
# ----------------------------------------------------------------------
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jelly)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/jelly)

set(API_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jelly_api)
set(API_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/jelly_api)

set(API_HEADER_FILES
    ${API_HEADER_DIR}/jelly_api_export.h
    ${API_HEADER_DIR}/jelly_handle.h
    ${API_HEADER_DIR}/jelly.h
    ${API_HEADER_DIR}/scene_manager.h
)

set(API_SOURCE_FILES
    ${API_SRC_DIR}/jelly.cpp
    ${API_SRC_DIR}/scene_manager.cpp
)

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
    ${HEADER_DIR}/graphics/graphic_api_interface.hpp
    ${HEADER_DIR}/graphics/graphic_api_factory.hpp
    ${HEADER_DIR}/graphics/graphic_context.hpp
    ${HEADER_DIR}/graphics/vulkan/vulkan_graphic_api.hpp
    ${HEADER_DIR}/graphics/vulkan/queue_family_indices.hpp
    ${HEADER_DIR}/graphics/vulkan/swap_chain_support_details.hpp
    ${HEADER_DIR}/windowing/window_system_interface.hpp
    ${HEADER_DIR}/windowing/native_window_handle_provider_interface.hpp
    ${HEADER_DIR}/windowing/vulkan_native_window_handle_provider.hpp
    ${HEADER_DIR}/windowing/window_system_factory.hpp
    ${HEADER_DIR}/windowing/window_graphic_api_binder.hpp
    ${HEADER_DIR}/windowing/glfw/glfw_window_system.hpp
    ${HEADER_DIR}/windowing/glfw/glfw_vulkan_window_system.hpp
    ${API_HEADER_FILES}
)

set(SRC_FILES
    ${SRC_DIR}/jelly.cpp
    ${SRC_DIR}/core/logger.cpp
    ${SRC_DIR}/core/scene.cpp
    ${SRC_DIR}/core/scene_manager.cpp
    ${SRC_DIR}/graphics/graphic_context.cpp
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
    ${SRC_DIR}/windowing/glfw/glfw_window_system.cpp
    ${SRC_DIR}/windowing/glfw/glfw_vulkan_window_system.cpp
    ${API_SOURCE_FILES}
)

add_library(Jelly SHARED ${HEADERS} ${SRC_FILES})

#add_executable(Jelly src/jelly/main.cpp)

# ----------------------------------------------------------------------
# Includes and Linking
# ----------------------------------------------------------------------
# Rename on MinGW (remove lib prefix)
if(WIN32 AND MINGW)
    set_target_properties(Jelly PROPERTIES
        ARCHIVE_OUTPUT_NAME "Jelly"
        PREFIX ""
    )
endif()

set_target_properties(Jelly PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

target_compile_definitions(Jelly PRIVATE JELLY_BUILD_DLL)

target_include_directories(Jelly PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Jelly PUBLIC
    glfw
    Vulkan::Vulkan
    EnTT
)