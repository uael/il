cmake_minimum_required(VERSION 2.6.2)

project(libparse)

find_package(BISON)
find_package(FLEX)

set(libparse_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${libparse_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

bison_target(parser parser.yy ${libparse_SOURCE_DIR}/parser.cc)
flex_target(scanner scanner.ll ${libparse_SOURCE_DIR}/scanner.cc)
add_flex_bison_dependency(scanner parser)

set(libparse_SOURCES
        ${FLEX_scanner_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        parser.yy
        scanner.ll
        y.tab.h
        driver.cc)

add_library(parse ${libparse_SOURCES})
target_link_libraries(parse ast)