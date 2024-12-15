set(LIBNAME node)

add_library(${LIBNAME} node.c)

set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)

target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(node dynamicArray)
