set(LIBNAME fileBuffer)

add_library(${LIBNAME} fileBuffer.c)
set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)
target_link_libraries(${LIBNAME} shader dynamicArray)

target_include_directories(${LIBNAME} PUBLIC .)
