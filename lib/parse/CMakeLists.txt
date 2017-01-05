find_package(BISON)
find_package(FLEX)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

bison_target(parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(scanner scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
add_flex_bison_dependency(scanner parser)

set(libparse_SOURCES
        ${FLEX_scanner_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        parser.yy
        scanner.ll
        y.tab.h
        driver.cc)

add_library(parse ${libparse_SOURCES})
target_link_libraries(parse ast cgen)