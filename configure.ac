AC_INIT([Jay], [0.1], [https://github.com/jayl/jay/issues])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_SRCDIR([src/driver.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE([foreign subdir-objects serial-tests])

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX(11, noext, mandatory)
AC_LANG([C])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LEX
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_BISON

LT_INIT

AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile test/Makefile])
AC_OUTPUT