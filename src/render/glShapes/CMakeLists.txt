add_library(glShapes STATIC)

add_subdirectory(src)

target_link_libraries(glShapes PRIVATE glad shader common)
target_include_directories(glShapes PUBLIC include)
target_include_directories(glShapes PRIVATE internal)
