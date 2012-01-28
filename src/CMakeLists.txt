cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("${PROJECT_SOURCE_DIR}/src")

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(python)