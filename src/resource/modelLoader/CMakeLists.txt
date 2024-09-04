set(LIBNAME modelLoader)

add_library(${LIBNAME} modelLoader.c)

set_property(TARGET ${LIBNAME} PROPERTY C_STANDARD 23)

target_link_libraries(${LIBNAME} PRIVATE glad assimp)
target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} .)
