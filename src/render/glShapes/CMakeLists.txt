set(LIBNAME glShapes)

add_subdirectory(src)

target_include_directories(${LIBNAME} PUBLIC include)
target_include_directories(${LIBNAME} PRIVATE internal)
