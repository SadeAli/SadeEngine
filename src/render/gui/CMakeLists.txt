add_library(gui gui.c)

set_property(TARGET gui PROPERTY C_STANDARD 23)

target_link_libraries(gui cimgui)

target_include_directories(gui PUBLIC .)
