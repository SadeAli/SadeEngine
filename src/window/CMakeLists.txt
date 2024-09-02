add_library(window window.c)

set_property(TARGET window PROPERTY C_STANDARD 23)

target_link_libraries(window glad cimgui glfw)

target_include_directories(window PUBLIC .)
