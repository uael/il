AM_DEFAULT_SOURCE_EXT = .cc
AM_LDFLAGS = $(top_builddir)/lib/libgtest.la $(top_builddir)/lib/parse/libparse.la
AM_CXXFLAGS = \
  -I$(top_srcdir)/lib/gtest/googletest/include \
  -I$(top_srcdir)/src -I$(top_builddir)/src \
  -I$(top_srcdir)/lib -I$(top_builddir)/lib -I$(top_builddir)/lib/parse

check_PROGRAMS = \
  argument function frame

TESTS = $(check_PROGRAMS)