/*
 * Wulk - Wu uniform language kit
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#ifndef   WULK_C_LEXER_H__
# define  WULK_C_LEXER_H__

#include "lexer.h"

enum {
  C_TOK_END = '\0',
  C_TOK_AUTO = C_TOK_END + 1,
  C_TOK_BREAK,
  C_TOK_CASE,
  C_TOK_CHAR,
  C_TOK_CONST,
  C_TOK_CONTINUE,
  C_TOK_DEFAULT,
  C_TOK_DO,
  C_TOK_DOUBLE,
  C_TOK_EOL = '\n',
  C_TOK_ELSE = C_TOK_EOL + 1,
  C_TOK_ENUM,
  C_TOK_EXTERN,
  C_TOK_FLOAT,
  C_TOK_FOR,
  C_TOK_GOTO,
  C_TOK_IF,
  C_TOK_INLINE,
  C_TOK_INT,
  C_TOK_LONG,
  C_TOK_REGISTER,
  C_TOK_RESTRICT,
  C_TOK_RETURN,
  C_TOK_SHORT,
  C_TOK_SIGNED,
  C_TOK_SIZEOF,
  C_TOK_STATIC,
  C_TOK_STRUCT,
  C_TOK_SWITCH,
  C_TOK_TYPEDEF,
  C_TOK_UNION,
  C_TOK_UNSIGNED,

  C_TOK_NOT = '!',
  C_TOK_VOID = C_TOK_NOT + 1,
  C_TOK_HASH = '#',
  C_TOK_VOLATILE = C_TOK_HASH + 1,
  C_TOK_MODULO = '%',
  C_TOK_AND = '&',
  C_TOK_WHILE = C_TOK_AND + 1,

  C_TOK_OPEN_PAREN = '(',
  C_TOK_CLOSE_PAREN = ')',
  C_TOK_STAR = '*',
  C_TOK_PLUS = '+',
  C_TOK_COMMA = ',',
  C_TOK_MINUS = '-',
  C_TOK_DOT = '.',
  C_TOK_SLASH = '/',

  C_TOK_ALIGNAS = C_TOK_SLASH + 1,
  C_TOK_ALIGNOF,
  C_TOK_ATOMIC,
  C_TOK_BOOL,
  C_TOK_COMPLEX,
  C_TOK_GENERIC,
  C_TOK_IMAGINARY,
  C_TOK_NORETURN,
  C_TOK_STATIC_ASSERT,
  C_TOK_THREAD_LOCAL,

  C_TOK_COLON = ':',
  C_TOK_SEMICOLON = ';',
  C_TOK_LT = '<',
  C_TOK_ASSIGN = '=',
  C_TOK_GT = '>',
  C_TOK_QUESTION = '?',
  C_TOK_DOTS = C_TOK_QUESTION + 1,  /* ... */
  C_TOK_LOGICAL_OR,           /* || */
  C_TOK_LOGICAL_AND,          /* && */
  C_TOK_LE_OP,                  /* <= */
  C_TOK_GE_OP,                  /* >= */
  C_TOK_EQ_OP,                   /* == */
  C_TOK_NE_OP,                  /* != */
  C_TOK_PTR_OP,                /* -> */
  C_TOK_INCREMENT,            /* ++ */
  C_TOK_DECREMENT,            /* -- */
  C_TOK_LEFT_OP,               /* << */
  C_TOK_RIGHT_OP,               /* >> */
  C_TOK_MUL_ASSIGN,           /* *= */
  C_TOK_DIV_ASSIGN,           /* /= */
  C_TOK_MOD_ASSIGN,           /* %= */
  C_TOK_PLUS_ASSIGN,          /* += */
  C_TOK_MINUS_ASSIGN,         /* -= */
  C_TOK_LSHIFT_ASSIGN,        /* <<= */
  C_TOK_RSHIFT_ASSIGN,        /* >>= */
  C_TOK_AND_ASSIGN,           /* &= */
  C_TOK_XOR_ASSIGN,           /* ^= */
  C_TOK_OR_ASSIGN,            /* |= */
  C_TOK_TOKEN_PASTE,          /* ## */

  C_TOK_FUNC_NAME,

  C_TOK_OPEN_BRACKET = '[',
  C_TOK_ANTI_SLASH = '\\',
  C_TOK_CLOSE_BRACKET = ']',
  C_TOK_XOR = '^',
  C_TOK_OPEN_CURLY = '{',
  C_TOK_OR = '|',
  C_TOK_CLOSE_CURLY = '}',
  C_TOK_NEG = '~',

  C_TOK_NUMBER = 116,
  C_TOK_IDENTIFIER = C_TOK_NUMBER + 1,
  C_TOK_STRING
};

void c_lexer_init(wulk_lexer_t *self);

#endif /* WULK_C_LEXER_H__ */
