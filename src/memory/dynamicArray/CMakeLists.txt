set(LIBNAME dynamicArray)

add_library(${LIBNAME} dynamicArray.c)

set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
