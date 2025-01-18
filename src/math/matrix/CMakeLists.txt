add_library(matrix matrix.c)

target_include_directories(matrix PUBLIC .)
target_link_libraries(matrix PUBLIC defines vec defines)
