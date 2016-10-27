# $Id$

AM_CFLAGS = -W -Wall -Wextra -ansi
AM_CXXFLAGS = -W -Wall -Wextra -ansi

noinst_LIBRARIES = libscc.a

libscc_a_SOURCES = \
	parser.yy scanner.ll parser.hh scanner.h y.tab.h \
	location.hh position.hh stack.hh \
	driver.cc driver.h expression.h

LIBS = libscc.a

bin_PROGRAMS = dcc

dcc_SOURCES = main.cc
dcc_DEPENDENCIES = libscc.a

AM_LFLAGS = -olex.yy.c