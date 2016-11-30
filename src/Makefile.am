AM_CFLAGS = -W -Wall -Wextra -ansi -Isrc
AM_CXXFLAGS = -W -Wall -Wextra -ansi -std=c++11 -Isrc
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

noinst_LTLIBRARIES = libdcc.la

libdcc_la_SOURCES = \
	parser.yy scanner.ll y.tab.h \
	ast.cc driver.cc

bin_PROGRAMS = ddc

ddc_SOURCES = main.cc
ddc_LDFLAGS = libdcc.la
ddc_DEPENDENCIES = libdcc.la

pkginclude_HEADERS = ddc.h