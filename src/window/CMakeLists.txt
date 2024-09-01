add_library(window window.c)

set_property(TARGET window PROPERTY C_STANDARD 23)
set_property(TARGET window PROPERTY CXX_STANDARD 17)

target_link_libraries(window glad cimgui glfw)

target_include_directories(window PUBLIC .)
