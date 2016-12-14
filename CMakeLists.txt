cmake_minimum_required(VERSION 3.5)
project(dyc)

set(CMAKE_CXX_STANDARD 11)

find_package(BISON)
find_package(FLEX)

bison_target(parser src/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(scanner src/scanner.ll  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
add_flex_bison_dependency(scanner parser)

include_directories(src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(libdyc_SOURCES
    src/ast/closure.cc
    src/ast/const.cc
    src/ast/decl.cc
    src/ast/expr.cc
    src/ast/generic.cc
    src/ast/ident.cc
    src/ast/node.cc
    src/ast/stmt.cc
    src/ast/type.cc
    src/ast.cc
    ${FLEX_scanner_OUTPUTS}
    ${BISON_parser_OUTPUTS}
    src/parser.yy
    src/scanner.ll
    src/y.tab.h
    src/driver.cc
    src/generator.cc)

set(dyc_SOURCES
    src/main.cc)

add_library(libdyc ${libdyc_SOURCES})
add_executable(dyc ${dyc_SOURCES})
target_link_libraries(dyc libdyc)

enable_testing()

add_subdirectory(lib/gtest)
include_directories(${gtest_SOURCE_DIR}/include)

function(check_tests)
    foreach(f ${ARGN})
        set(${f}_SOURCES tests/${f}.cc)
        add_executable(${f} ${${f}_SOURCES})
        target_link_libraries(${f} libdyc gtest_main)
        set(check_PROGRAMS ${check_PROGRAMS} COMMAND ${f})
    endforeach()
    add_custom_target(check ${check_PROGRAMS})
endfunction()

check_tests(argument function)