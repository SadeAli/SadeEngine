add_library(math INTERFACE)

add_subdirectory(color)
add_subdirectory(vector)
add_subdirectory(matrix)

target_link_libraries(math INTERFACE color vec vector)
target_include_directories(math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
