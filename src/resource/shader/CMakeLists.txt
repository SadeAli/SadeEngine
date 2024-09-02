set(LIBNAME shader)

add_library(${LIBNAME} shader.c)

set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)

target_link_libraries(${LIBNAME} PRIVATE glad fileBuffer)
target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} .)
