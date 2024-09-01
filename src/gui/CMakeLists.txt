add_library(gui gui.c)

set_property(TARGET gui PROPERTY C_STANDARD 23)
set_property(TARGET gui PROPERTY CXX_STANDARD 17)

target_link_libraries(gui cimgui)

target_include_directories(gui PUBLIC .)
