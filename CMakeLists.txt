cmake_minimum_required(VERSION 3.5)
project(dyc)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)