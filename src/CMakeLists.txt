include_directories(${libfirm_INCLUDE_DIRS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wno-missing-braces -Wno-psabi")

set(jay_SOURCES
    ${jayl_INCLUDES}
    backend/graphviz/dot.c
    backend/x86_64/abi.c
    backend/x86_64/assemble.c
    backend/x86_64/elf.c
    backend/x86_64/instr.c
    backend/compile.c
    optimizer/liveness.c
    optimizer/optimize.c
    optimizer/transform.c
    parser/declaration.c
    parser/eval.c
    parser/expression.c
    parser/parse.c
    parser/expression.c
    parser/parse.c
    parser/statement.c
    parser/symtab.c
    parser/typetree.c
    preprocessor/directive.c
    preprocessor/input.c
    preprocessor/macro.c
    preprocessor/preprocess.c
    preprocessor/strtab.c
    preprocessor/tokenize.c
    util/argparse.c
    util/hash.c
    util/string.c
    context.c
    main.c
)

add_executable(jay ${jay_SOURCES})
target_link_libraries(jay firm)