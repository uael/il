AM_CFLAGS = -Wall -I$(top_srcdir)/lib
AM_CXXFLAGS = -Wall -std=c++11 -I$(top_srcdir)/lib

check_LTLIBRARIES = libgtest.la

libgtest_la_SOURCES = gtest/googletest/src/gtest-all.cc
libgtest_la_CXXFLAGS = -I$(top_srcdir)/lib/gtest/googletest/include -I$(top_srcdir)/lib/gtest/googletest
libgtest_la_LDFLAGS = -pthread