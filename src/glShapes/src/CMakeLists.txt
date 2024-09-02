add_library(${LIBNAME} grid.c line.c rect.c)
target_link_libraries(${LIBNAME} glad raylib)

set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)
