AM_CFLAGS = -W -Wall -Wextra -ansi -Isrc
AM_CXXFLAGS = -W -Wall -Wextra -ansi -std=c++11 -Isrc
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

noinst_LTLIBRARIES = libdcc.la

libdcc_la_SOURCES = \
	parser.yy scanner.ll y.tab.h \
	driver.cc generator.cc

bin_PROGRAMS = dyc

dyc_SOURCES = main.cc
dyc_LDFLAGS = libdcc.la
dyc_DEPENDENCIES = libdcc.la

pkginclude_HEADERS = dyc.h