AM_CFLAGS = -Wall -Isrc
AM_CXXFLAGS = -Wall -std=c++11 -Isrc
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

noinst_LTLIBRARIES = libdyc.la

libdyc_la_SOURCES = \
	ast/closure.cc ast/const.cc ast/decl.cc ast/expr.cc ast/generic.cc \
	ast/ident.cc ast/node.cc ast/stmt.cc ast/type.cc \
	ast.cc \
	parser.yy scanner.ll y.tab.h \
	driver.cc generator.cc

bin_PROGRAMS = dyc

dyc_SOURCES = main.cc
dyc_LDADD = libdyc.la

pkginclude_HEADERS = dyc.h