cmake_minimum_required(VERSION 3.5)
project(jay)

set(CMAKE_CXX_STANDARD 11)

include_directories(lib src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib/parse)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)