/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!@file il/mir/ir.h
 * @author uael
 */
#ifndef __IL_MIR_IR_H
# define __IL_MIR_IR_H

#define vecof(T) struct { T *buf; unsigned long len; }

enum il_mir_aggkind {
  IL_MIR_AGG_ARRAY = 0,
  IL_MIR_AGG_TUPLE,
  IL_MIR_AGG_ADT,
  IL_MIR_AGG_CLOSURE
};

enum il_mir_binopkind {
  IL_MIR_BINOP_ADD = 0,
  IL_MIR_BINOP_SUB,
  IL_MIR_BINOP_MUL,
  IL_MIR_BINOP_DIV,
  IL_MIR_BINOP_REM,
  IL_MIR_BINOP_XOR,
  IL_MIR_BINOP_AND,
  IL_MIR_BINOP_OR,
  IL_MIR_BINOP_SHL,
  IL_MIR_BINOP_SHR,
  IL_MIR_BINOP_EQ,
  IL_MIR_BINOP_LT,
  IL_MIR_BINOP_LE,
  IL_MIR_BINOP_NE,
  IL_MIR_BINOP_GE,
  IL_MIR_BINOP_GT,
  IL_MIR_BINOP_OFF
};

enum il_mir_borrowkind {
  IL_MIR_BORROW_SHARED = 0,
  IL_MIR_BORROW_UNIQUE,
  IL_MIR_BORROW_MUT
};

enum il_mir_castkind {
  IL_MIR_CAST_MISC = 0,
  IL_MIR_CAST_REIFYFNPOINTER,
  IL_MIR_CAST_CLOSUREFNPOINTER,
  IL_MIR_CAST_UNSAFEFNPOINTER,
  IL_MIR_CAST_UNSIZE
};

enum il_mir_litkind {
  IL_MIR_LIT_ITEM = 0,
  IL_MIR_LIT_VALUE,
  IL_MIR_LIT_PROMOTED
};

enum il_mir_localkind {
  IL_MIR_LOCAL_VAR = 0,
  IL_MIR_LOCAL_TEMP,
  IL_MIR_LOCAL_ARG,
  IL_MIR_LOCAL_RETURNPTR
};

enum il_mir_lvaluekind {
  IL_MIR_LVALUE_LOCAL = 0,
  IL_MIR_LVALUE_STATIC,
  IL_MIR_LVALUE_PROJECTION
};

enum il_mir_mutability {
  IL_MIR_MUTABILITY_MUT = 0,
  Il_MIR_MUTABILITY_NOT
};

enum il_mir_nullopkind {
  IL_MIR_NULLOP_SIZEOF = 0,
  IL_MIR_NULLOP_BOX
};

enum il_mir_opkind {
  IL_MIR_OP_CONSUME = 0,
  IL_MIR_OP_CONSTANT
};

enum il_mir_projelmkind {
  IL_MIR_PROJELM_DEREF,
  IL_MIR_PROJELM_FIELD,
  IL_MIR_PROJELM_INDEX,
  IL_MIR_PROJELM_CONSTIDX,
  IL_MIR_PROJELM_SUBSLICE,
  IL_MIR_PROJELM_DOWNCAST
};

enum il_mir_rvaluekind {
  IL_MIR_RVALUE_USE = 0,
  IL_MIR_RVALUE_REPEAT,
  IL_MIR_RVALUE_REF,
  IL_MIR_RVALUE_LEN,
  IL_MIR_RVALUE_CAST,
  IL_MIR_RVALUE_BINOP,
  IL_MIR_RVALUE_CHECKED_BINOP,
  IL_MIR_RVALUE_NULLOP,
  IL_MIR_RVALUE_UNOP,
  IL_MIR_RVALUE_DISCR,
  IL_MIR_RVALUE_AGG
};

enum il_mir_stmtkind {
  IL_MIR_STMT_ASSIGN = 0,
  IL_MIR_STMT_DISCR,
  IL_MIR_STMT_STORAGE_LIVE,
  IL_MIR_STMT_STORAGE_DEAD,
  IL_MIR_STMT_ASM,
  IL_MIR_STMT_VALIDATE,
  IL_MIR_STMT_ENDREGION,
  IL_MIR_STMT_NOP
};

enum il_mir_termkind {
  IL_MIR_TERM_GOTO = 0,
  IL_MIR_TERM_SWITCH,
  IL_MIR_TERM_RESUME,
  IL_MIR_TERM_RETURN,
  IL_MIR_TERM_UNREACHABLE,
  IL_MIR_TERM_DROP,
  IL_MIR_TERM_DROPNREPLACE,
  IL_MIR_TERM_CALL,
  IL_MIR_TERM_ASSERT
};

enum il_mir_unopkind {
  IL_MIR_UNOP_NOT = 0,
  IL_MIR_UNOP_NEG
};

enum il_mir_validateopkind {
  IL_MIR_VALIDATEOP_ACQUIRE = 0,
  IL_MIR_VALIDATEOP_RELEASE,
  IL_MIR_VALIDATEOP_SUSPEND
};

union il_mir_agg {
  enum il_mir_aggkind kind;
  struct {
    enum il_mir_aggkind kind;
    struct il_ty ty;
  } array;
  struct {
    enum il_mir_aggkind kind;
    struct il_ty_adtdef adtdef;
    unsigned long discr;
    struct il_ty_substs substs;
    unsigned long fnb;
  } adt;
};

struct il_mir_promoted {
  int idx;
};

union il_mir_lit {
  enum il_mir_litkind kind;
  struct {
    struct il_defid defid;
    struct il_ty_substs substs;
  } item;
  struct il_val value;
  struct il_mir_promoted promoted;
};

struct il_mir_block {
  vecof(struct il_mir_stmt) stmts;
  struct il_mir_term *term;
  unsigned char is_cleanup;
};

struct il_mir_const {
  struct il_syn_span span;
  struct il_ty ty;
  union il_mir_lit lit;
};

struct il_mir_loc {
  struct il_mir_block block;
  unsigned long idx;
};

#endif /* !__IL_MIR_IR_H */
