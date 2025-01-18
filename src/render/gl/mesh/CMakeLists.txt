add_library(mesh SHARED mesh2d.c templateMesh.c)
target_link_libraries(mesh glad vector)
target_include_directories(mesh PUBLIC .)
