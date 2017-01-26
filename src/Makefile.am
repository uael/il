AM_CFLAGS = -I$(top_srcdir)/include/ -pedantic -Wno-missing-braces -Wno-psabi

noinst_LTLIBRARIES = libjay.la

libjay_la_SOURCES = \
  backend/graphviz/dot.c \
  backend/x86_64/abi.c \
  backend/x86_64/assemble.c \
  backend/x86_64/elf.c \
  backend/x86_64/instr.c \
  backend/compile.c \
  optimizer/liveness.c \
  optimizer/optimize.c \
  optimizer/transform.c \
  parser/declaration.c \
  parser/eval.c \
  parser/expression.c \
  parser/parse.c \
  parser/expression.c \
  parser/parse.c \
  parser/statement.c \
  parser/symtab.c \
  parser/typetree.c \
  preprocessor/directive.c \
  preprocessor/input.c \
  preprocessor/macro.c \
  preprocessor/preprocess.c \
  preprocessor/strtab.c \
  preprocessor/tokenize.c \
  util/argparse.c \
  util/hash.c \
  util/string.c \
  context.c

bin_PROGRAMS = jay

jay_SOURCES = main.c
jay_LDADD = libjay.la
