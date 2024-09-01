set(LIBNAME glShapes)

# add library called inside this directory
add_subdirectory(shapes)

target_include_directories(${LIBNAME} PRIVATE internal)

set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${LIBNAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${LIBNAME} glad raylib)

target_include_directories(${LIBNAME} PUBLIC .)
