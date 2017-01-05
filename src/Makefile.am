AM_CFLAGS = -Wall -Isrc
AM_CXXFLAGS = -Wall -std=c++11 -Isrc
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

noinst_LTLIBRARIES = libast.la

libast_la_SOURCES = \
	ast/closure.cc ast/const.cc ast/decl.cc ast/expr.cc ast/file.cc \
	ast/generic.cc ast/ident.cc ast/node.cc ast/stmt.cc ast/type.cc \
	program.cc \
	gen/ast_gen.cc gen/file_gen.cc \
	parser.yy scanner.ll y.tab.h \
	driver.cc

bin_PROGRAMS = jay

jay_SOURCES = main.cc
jay_LDADD = libast.la

pkginclude_HEADERS = jay.h