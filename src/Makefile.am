AM_CFLAGS = -Wall -I$(top_srcdir)/lib -I$(top_builddir)/lib/parse
AM_CXXFLAGS = -Wall -std=c++11 -I$(top_srcdir)/lib -I$(top_builddir)/lib/parse
AM_LDFLAGS = $(top_builddir)/lib/parse/libparse.la \
  $(top_builddir)/lib/ast/libast.la $(top_builddir)/lib/cgen/libcgen.la

bin_PROGRAMS = jay

jay_SOURCES = jay.cc

pkginclude_HEADERS = jay.h