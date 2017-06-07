#
# Wulk - Wu uniform language kit
# Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, see <http://www.gnu.org/licenses/>
#

cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(il)

option(COVERAGE "Turn on COVERAGE support" OFF)

if(COVERAGE AND NOT MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif()

if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
    endif()
else()
    set(CMAKE_C_STANDARD 99)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
else()
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -DDEBUG")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Werror -fomit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-function")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable -Wno-unused-value -Wno-unused-result -Wno-missing-braces")
endif()

if(MSVC)
    set(CMAKE_FLAGS
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
    )
    foreach(CMAKE_FLAG ${CMAKE_FLAGS})
        string(REPLACE "/MD" "/MT" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
        string(REPLACE "/MDd" "/MTd" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
    endforeach()
endif()

set(il_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(il_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(il_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
set(il_VENDOR_DIR ${CMAKE_CURRENT_LIST_DIR}/vendor)

file(GLOB_RECURSE il_HEADERS ${il_HEADERS} ${il_INCLUDE_DIR}/*.h)
file(GLOB_RECURSE il_SOURCES ${il_SOURCES} ${il_SOURCE_DIR}/*.c)

add_library(${PROJECT_NAME} STATIC ${il_SOURCES} ${il_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${il_HEADERS}")
target_include_directories(${PROJECT_NAME} PUBLIC ${il_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${il_INCLUDE_DIR}/${PROJECT_NAME} ${il_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC u)
add_dependencies(${PROJECT_NAME} u)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC winmm)
else()
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC m)
endif()

add_subdirectory(${il_VENDOR_DIR})
if(EXISTS ${il_TEST_DIR})
    enable_testing()
    add_subdirectory(${il_TEST_DIR})
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
