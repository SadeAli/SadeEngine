set(LIBNAME camera3d)

add_library(${LIBNAME} camera3d.c)
target_link_libraries(${LIBNAME} PRIVATE cglm)

target_include_directories(${LIBNAME} PUBLIC .)
