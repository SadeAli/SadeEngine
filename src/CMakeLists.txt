# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# include_directories(${PROJECT_SOURCE_DIR}/src/math)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# DEEPRECATED set(SOURCES example_3d.c) add_executable(${PROJECT_NAME}
# ${SOURCES})

include_directories(
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/external/gladFiles
  ${EXTERNAL_DEPENDENCY_PATH} ${PROJECT_SOURCE_DIR}/memory)

# Add subdirectories add_subdirectory(relativePosition)
# add_subdirectory(editorClasses) add_subdirectory(vector)
add_subdirectory(window)
add_subdirectory(dataStructures)
add_subdirectory(render)
add_subdirectory(resource)
add_subdirectory(fileBuffer)

target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -fsanitize=address)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC window
         m
         camera3d
         cglm
         glad
         assimp
         gui
         glShapes
         dynamicArray
         shaderManager
         shader
         modelLoader
         -fsanitize=address)

# Disable treating warnings as errors for Assimp
target_compile_options(assimp PRIVATE -Wno-error)

# Add resources add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND
# ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/../resources
# $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
