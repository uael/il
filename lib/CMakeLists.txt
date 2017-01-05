add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include)

set(libast_SOURCES
        ast/ast.h
        ast/closure.cc
        ast/const.cc
        ast/decl.cc
        ast/expr.cc
        ast/file.cc
        ast/generic.cc
        ast/ident.cc
        ast/node.cc
        ast/program.cc
        ast/stmt.cc
        ast/type.cc)
add_library(ast ${libast_SOURCES})

set(libcgen_SOURCES
        cgen/cgen.h
        cgen/ast_gen.cc
        cgen/file_gen.cc)
add_library(cgen ${libcgen_SOURCES})
target_link_libraries(cgen ast)

include_directories(parse)
add_subdirectory(parse)