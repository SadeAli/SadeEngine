cmake_minimum_required(VERSION 3.21)
project(sadeEngine)

# set(CC clang)
set(CMAKE_C_COMPILER_ID "clang")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cimgui.cmake)
add_subdirectory(external)
add_subdirectory(src)
