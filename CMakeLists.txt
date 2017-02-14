cmake_minimum_required(VERSION 2.8.7)
project(jay)

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

set(jayl_INCLUDE_DIRS include include/jayl include/stdlib)
foreach(dir ${jayl_INCLUDE_DIRS})
    file(GLOB jayl_HEADERS ${jayl_HEADERS} ${dir}/*.h)
endforeach()

include_directories(include)
include_directories(lib/p99/p99)

add_subdirectory(src)
add_subdirectory(test)
