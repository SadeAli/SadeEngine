set(LIBNAME shaderManager)

add_library(${LIBNAME} shaderManager.c)
set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)
target_link_libraries(shaderManager shader dynamicArray)

target_include_directories(${LIBNAME} PUBLIC .)
