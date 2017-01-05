find_package(BISON)
find_package(FLEX)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

bison_target(parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(scanner scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
add_flex_bison_dependency(scanner parser)

set(libast_SOURCES
        ast/closure.cc
        ast/const.cc
        ast/decl.cc
        ast/expr.cc
        ast/file.cc
        ast/generic.cc
        ast/ident.cc
        ast/node.cc
        ast/stmt.cc
        ast/type.cc
        program.cc
        gen/ast_gen.cc
        gen/file_gen.cc
        ${FLEX_scanner_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        parser.yy
        scanner.ll
        y.tab.h
        driver.cc)

add_library(ast ${libast_SOURCES})

set(jay_SOURCES
        main.cc)

add_executable(jay ${jay_SOURCES})
target_link_libraries(jay ast)