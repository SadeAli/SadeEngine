add_library(color color.c)

target_include_directories(color PUBLIC .)
target_link_libraries(color PUBLIC defines)
