add_subdirectory(glShapes)
add_subdirectory(gui)
add_subdirectory(camera)
target_sources(${PROJECT_NAME} PRIVATE drawable.c)
