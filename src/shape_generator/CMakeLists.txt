add_library(shape_generator STATIC)

set_property(TARGET shape_generator PROPERTY C_STANDARD 17)

add_subdirectory(shapes)

target_link_libraries(shape_generator PRIVATE vector m)
target_include_directories(shape_generator PUBLIC .)
