AM_CFLAGS = -Wall -I$(top_srcdir)/lib -I$(top_builddir)/lib/parse
AM_CXXFLAGS = -Wall -std=c++11 -I$(top_srcdir)/lib -I$(top_builddir)/lib/parse

bin_PROGRAMS = jay

jay_SOURCES = jay.cc
jay_LDADD = $(top_builddir)/lib/parse/libparse.la

pkginclude_HEADERS = jay.h