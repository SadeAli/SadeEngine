include(${PROJECT_SOURCE_DIR}/cmake/cimgui.cmake)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/memory)
include_directories(${PROJECT_SOURCE_DIR}/src/external)
include_directories(${PROJECT_SOURCE_DIR}/src/external/gladFiles)
# include_directories(${PROJECT_SOURCE_DIR}/src/math)

# Source files
set(SOURCES main.c)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add subdirectories add_subdirectory(relativePosition)
# add_subdirectory(editorClasses) add_subdirectory(vector)
add_subdirectory(external)
add_subdirectory(window)
add_subdirectory(memory)
add_subdirectory(render)
add_subdirectory(resource)
add_subdirectory(fileBuffer)
add_subdirectory(camera)

target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -fsanitize=address)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE window
          m
          cglm
          cimgui
          assimp
          gui
          glShapes
          dynamicArray
          shaderManager
          shader
          modelLoader
          -fsanitize=address)

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
