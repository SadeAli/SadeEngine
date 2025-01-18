add_library(gui gui.c)

target_link_libraries(gui PRIVATE cimgui)
target_include_directories(gui PUBLIC .)
