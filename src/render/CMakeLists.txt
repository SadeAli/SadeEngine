add_subdirectory(glShapes)
add_subdirectory(gui)
add_subdirectory(camera)

add_library(render drawable.c render.c)
target_link_libraries(render PUBLIC gui glShapes camera stb glad)
