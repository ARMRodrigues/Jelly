cmake_minimum_required (VERSION 3.16)

project(Jelly CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# ----------------------------------------------------------------------
# Library
# ----------------------------------------------------------------------
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jelly)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/jelly)

set(API_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jelly_api)
set(API_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/jelly_api)

set(API_HEADER_FILES
    ${API_HEADER_DIR}/jelly_api_export.h
    ${API_HEADER_DIR}/jelly.h
)

set(API_SOURCE_FILES
    ${API_SRC_DIR}/jelly.cpp
)

set(HEADERS
    ${HEADER_DIR}/jelly.hpp
    ${HEADER_DIR}/core/managed_resource.hpp
    ${HEADER_DIR}/core/window_settings.hpp
    ${HEADER_DIR}/core/graphics_api_type.hpp
    ${HEADER_DIR}/windowing/window_system_interface.hpp
    ${HEADER_DIR}/windowing/window_system_factory.hpp
    ${HEADER_DIR}/windowing/glfw/glfw_window_system.hpp
    ${HEADER_DIR}/windowing/glfw/glfw_vulkan_window_system.hpp
    ${API_HEADER_FILES}
)

set(SRC_FILES
    ${SRC_DIR}/jelly.cpp
    ${SRC_DIR}/windowing/glfw/glfw_window_system.cpp
    ${SRC_DIR}/windowing/glfw/glfw_vulkan_window_system.cpp
    ${API_SOURCE_FILES}
)

add_library(Jelly SHARED ${HEADERS} ${SRC_FILES})

# ----------------------------------------------------------------------
# Includes and Linking
# ----------------------------------------------------------------------
target_compile_definitions(Jelly PRIVATE JELLY_BUILD_DLL)

target_include_directories(Jelly PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Jelly PUBLIC
    glfw
    Vulkan::Vulkan
)