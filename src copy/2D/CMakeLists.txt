# Source files
set(2D_SOURCES TopdownPlayer.c)

# Create a library
add_library(TopdownPlayer2D ${2D_SOURCES})

# Include directories
target_include_directories(TopdownPlayer2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
