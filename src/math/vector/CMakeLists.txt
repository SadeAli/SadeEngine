add_library(vec vec2.c vec3.c)
target_link_libraries(vec PUBLIC defines common)
target_include_directories(vec PUBLIC .)

add_library(vector vector2.c vector3.c)
target_link_libraries(vector PUBLIC defines vectorArray common)
target_include_directories(vector PUBLIC .)

add_subdirectory(array)
