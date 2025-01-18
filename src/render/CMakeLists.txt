add_subdirectory(glShapes)
add_subdirectory(gui)
add_subdirectory(camera)
add_subdirectory(gl)

add_library(render STATIC drawable.c render.c)
target_link_libraries(
  render
  PUBLIC gui
         glShapes
         camera
         stb
         glad
         color
         math
         mesh
)
