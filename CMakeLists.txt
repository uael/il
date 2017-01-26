cmake_minimum_required(VERSION 3.0)
project(jay)

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

set(jayl_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/array.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/context.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/deque.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/hash.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/ir.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/string.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/symbol.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/token.h
    ${CMAKE_CURRENT_LIST_DIR}/include/jayl/type.h
    ${CMAKE_CURRENT_LIST_DIR}/include/stdlib/float.h
    ${CMAKE_CURRENT_LIST_DIR}/include/stdlib/stdarg.h
    ${CMAKE_CURRENT_LIST_DIR}/include/stdlib/stddef.h
)

include_directories(include lib src)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)