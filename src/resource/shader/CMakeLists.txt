add_library(shader shader.c)

target_link_libraries(shader PUBLIC fileBuffer glad)
target_include_directories(shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
