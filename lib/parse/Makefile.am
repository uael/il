AM_CFLAGS = -Wall -I$(top_srcdir)/lib
AM_CXXFLAGS = -Wall -std=c++11 -I$(top_srcdir)/lib
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

noinst_LTLIBRARIES = libparse.la

libparse_la_SOURCES = parser.yy scanner.ll y.tab.h driver.cc
libparse_la_LIBADD = $(top_builddir)/lib/ast/libast.la $(top_builddir)/lib/cgen/libcgen.la