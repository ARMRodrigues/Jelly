cmake_minimum_required(VERSION 3.16)

project(JellyProject LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Parent output directory for all targets
get_filename_component(PARENT_DIR "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(OUTPUT_DIR "${PARENT_DIR}/Output/${CMAKE_BUILD_TYPE}" CACHE STRING "Output directory")

# Subprojects

add_subdirectory(Jelly)
add_subdirectory(Runtime)
