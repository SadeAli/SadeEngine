# Source files
set(3D_SOURCES CameraDefaults.c)  # Add your 3D source files here

# Create a library
add_library(3D ${3D_SOURCES})

# Include directories
target_include_directories(3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
