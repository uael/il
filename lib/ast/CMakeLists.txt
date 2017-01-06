cmake_minimum_required(VERSION 2.6.2)

project(libast)

set(libast_SOURCES
        ast.h
        closure.cc
        const.cc
        decl.cc
        expr.cc
        file.cc
        generic.cc
        ident.cc
        node.cc
        program.cc
        stmt.cc
        type.cc)
add_library(ast ${libast_SOURCES})