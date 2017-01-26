AC_INIT([jay], [0.1], [https://github.com/jayl/jay/issues])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE([foreign subdir-objects serial-tests])

AC_LANG([C])
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CXX

LT_INIT

AC_CONFIG_FILES([ \
  Makefile \
  lib/Makefile \
  src/Makefile \
  test/Makefile \
])
AC_OUTPUT
