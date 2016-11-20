AM_DEFAULT_SOURCE_EXT = .cc
AM_LDFLAGS = $(top_builddir)/lib/libgtest.la $(top_builddir)/src/libdcc.la
AM_CPPFLAGS = \
  -I$(top_srcdir)/lib/gtest/googletest/include \
  -I$(top_srcdir)/src -I$(top_builddir)/src \
  -I$(top_srcdir)/tests

check_PROGRAMS = \
  interface func

TESTS = $(check_PROGRAMS)