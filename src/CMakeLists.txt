# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Add interface library
add_library(${PROJECT_NAME} INTERFACE)
add_library(defines INTERFACE)

target_include_directories(defines INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Include directories
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
            ${PROJECT_SOURCE_DIR}/external/gladFiles
            ${EXTERNAL_DEPENDENCY_PATH}
            ${PROJECT_SOURCE_DIR}/memory
            ${PROJECT_SOURCE_DIR}/external/stb # Ensure STB directory is added
)

# Add subdirectories
add_subdirectory(window)
add_subdirectory(dataStructures)
add_subdirectory(render)
add_subdirectory(resource)
add_subdirectory(fileBuffer)

# Link libraries
target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE window
            m
            cglm
            glad
            assimp
            render
            dynamicArray
            shaderManager
            shader
            modelLoader
            -fsanitize=address)

# Ensure warnings are not treated as errors for Assimp
target_compile_options(assimp PRIVATE -Wno-error)

# Set C and C++ standards
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add resources add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND
# ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/../resources
# $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
