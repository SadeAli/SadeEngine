include(${PROJECT_SOURCE_DIR}/cmake/cimgui.cmake)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/memory)
include_directories(${PROJECT_SOURCE_DIR}/src/external)
include_directories(${PROJECT_SOURCE_DIR}/src/external/gladFiles)
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

target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw raylib m cglm cimgui assimp)

add_subdirectory(external/cglm)
add_subdirectory(external/glfw)
add_subdirectory(external/assimp)

# Disable treating warnings as errors for Assimp
target_compile_options(assimp PRIVATE -Wno-error)

# Add resources
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
