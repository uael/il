cmake_minimum_required(VERSION 2.8.12)
project(jay)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c11 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 11)
endif()

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(test)
