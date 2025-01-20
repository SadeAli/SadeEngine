cmake_minimum_required(VERSION 3.20)
project(ballsam)

# Set the path to SadeEngine
set(SADE_ENGINE_PATH ../../)

add_executable(${PROJECT_NAME} src/main.c)

# Add SadeEngine to the project
add_subdirectory(${SADE_ENGINE_PATH} sadeEngine)

# Include SadeEngine headers
include_directories(${SADE_ENGINE_PATH})

target_link_libraries(${PROJECT_NAME} sadeEngine -fsanitize=address)

target_compile_options(${PROJECT_NAME} PRIVATE -O0 -Wunused -Wunreachable-code -Wpedantic -Wunused-function -Wunused-variable)
