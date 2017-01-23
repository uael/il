AM_DEFAULT_SOURCE_EXT = .cc
AM_LDFLAGS = $(top_builddir)/lib/libgtest.la
AM_CXXFLAGS = \
  -I$(top_srcdir)/lib/gtest/googletest/include \
  -I$(top_srcdir)/src -I$(top_builddir)/src \
  -I$(top_srcdir)/lib -I$(top_builddir)/lib

check_PROGRAMS = \
  001

TESTS = $(check_PROGRAMS)