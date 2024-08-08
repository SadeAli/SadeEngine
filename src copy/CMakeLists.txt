# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/2D)
include_directories(${PROJECT_SOURCE_DIR}/src/3D)

# Add subdirectories
add_subdirectory(2D)
add_subdirectory(3D)

# Source files
set(SOURCES main.c)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} raylib TopdownPlayer2D 3D m)

# Add resources
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
