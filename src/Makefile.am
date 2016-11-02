AM_CFLAGS = -W -Wall -Wextra -ansi
AM_CXXFLAGS = -W -Wall -Wextra -ansi -std=c++11
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

noinst_LIBRARIES = libdcc.a

libdcc_a_SOURCES = \
	parser.yy scanner.ll y.tab.h \
	driver.cc ast.cc context.cc

LIBS = libdcc.a

bin_PROGRAMS = ddc

ddc_SOURCES = main.cc
ddc_DEPENDENCIES = libdcc.a

pkginclude_HEADERS = ddc.h