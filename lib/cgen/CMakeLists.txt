cmake_minimum_required(VERSION 2.6.2)

project(libcgen)

set(libcgen_SOURCES
        cgen.h
        closure.cc
        const.cc
        decl.cc
        expr.cc
        file.cc
        generic.cc
        id.cc
        node.cc
        program.cc
        stmt.cc
        type.cc)

add_library(cgen ${libcgen_SOURCES})
target_link_libraries(cgen ast)