cmake_minimum_required(VERSION 3.21)
project(sadeEngine)

# set(CC clang)
set(CMAKE_C_COMPILER_ID "clang")

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Add subdirectories for source files
add_subdirectory(src)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_compile_options(${PROJECT_NAME} PRIVATE -g)

add_custom_target(
  FinalMessage ALL
  ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Compilation is over!"
  COMMENT "compiler: ${CMAKE_C_COMPILER_ID}")
add_dependencies(FinalMessage ${PROJECT_NAME})
