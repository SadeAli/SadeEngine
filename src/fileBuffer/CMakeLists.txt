set(LIBNAME fileBuffer)

add_library(${LIBNAME} fileBuffer.c)
target_link_libraries(${LIBNAME} shader)

target_include_directories(${LIBNAME} PUBLIC .)
