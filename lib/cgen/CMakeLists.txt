cmake_minimum_required(VERSION 2.6.2)

project(libcgen)

set(libcgen_SOURCES
        cgen.h
        file.cc
        program.cc)

add_library(cgen ${libcgen_SOURCES})
target_link_libraries(cgen ast)