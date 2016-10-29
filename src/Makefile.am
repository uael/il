# $Id$

AM_CFLAGS = -W -Wall -Wextra -ansi
AM_CXXFLAGS = -W -Wall -Wextra -ansi

noinst_LIBRARIES = libdcc.a

libdcc_a_SOURCES = \
	parser.yy scanner.ll parser.hh scanner.h y.tab.h \
	location.hh position.hh stack.hh \
	driver.cc driver.h expression.h

LIBS = libdcc.a

bin_PROGRAMS = ddc

ddc_SOURCES = main.cc
ddc_DEPENDENCIES = libdcc.a

AM_LFLAGS = -olex.yy.c