# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/memory)
include_directories(${PROJECT_SOURCE_DIR}/src/external)
# include_directories(${PROJECT_SOURCE_DIR}/src/math)

# Source files
set(SOURCES main.c window.c)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add subdirectories add_subdirectory(relativePosition)
# add_subdirectory(editorClasses) add_subdirectory(vector)
add_subdirectory(memory)
# add_subdirectory(2d)
add_subdirectory(external)
add_subdirectory(render)
add_subdirectory(io)
add_subdirectory(utils)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wunused -g -O0)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw raylib m cglm)

add_subdirectory(external/cglm/)

# Add resources
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
