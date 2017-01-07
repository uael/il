AM_CFLAGS = -Wall -I$(top_srcdir)/lib
AM_CXXFLAGS = -Wall -std=c++11 -I$(top_srcdir)/lib

noinst_LTLIBRARIES = libcgen.la

libcgen_la_SOURCES = cgen.h file.cc program.cc
libcgen_la_LIBADD = $(top_builddir)/lib/ast/libast.la