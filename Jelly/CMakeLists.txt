cmake_minimum_required (VERSION 3.16)

project(Jelly CXX)

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jelly)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/jelly)

set(HEADERS
    ${HEADER_DIR}/jelly.h
)

set(SRC_FILES
    ${SRC_DIR}/jelly.cpp
)

add_library(Jelly SHARED ${HEADERS} ${SRC_FILES})

target_include_directories(Jelly
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)