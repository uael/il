/*
 * MIT License
 *
 * Copyright (c) 2016-2017 uael <www.github.com/uael>
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

#ifndef   JL_C_LEXER_H__
# define  JL_C_LEXER_H__

#include "lexer.h"

enum {
  C_TOK_END = 0,
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
  C_TOK_VOID,

  C_TOK_NOT = '!',
  C_TOK_VOLATILE = C_TOK_NOT + 1,
  C_TOK_HASH = '#',
  C_TOK_WHILE = C_TOK_HASH + 1,
  C_TOK_MODULO = '%',
  C_TOK_AND = '&',
  C_TOK_ALIGNOF = C_TOK_AND + 1,

  C_TOK_OPEN_PAREN = '(',
  C_TOK_CLOSE_PAREN = ')',
  C_TOK_STAR = '*',
  C_TOK_PLUS = '+',
  C_TOK_COMMA = ',',
  C_TOK_MINUS = '-',
  C_TOK_DOT = '.',
  C_TOK_SLASH = '/',
  C_TOK_COLON = ':',
  C_TOK_SEMICOLON = ';',
  C_TOK_LT = '<',
  C_TOK_ASSIGN = '=',
  C_TOK_GT = '>',
  C_TOK_QUESTION = '?',
  C_TOK_DOTS = C_TOK_QUESTION + 1,  /* ... */
  C_TOK_LOGICAL_OR,           /* || */
  C_TOK_LOGICAL_AND,          /* && */
  C_TOK_LEQ,                  /* <= */
  C_TOK_GEQ,                  /* >= */
  C_TOK_EQ,                   /* == */
  C_TOK_NEQ,                  /* != */
  C_TOK_ARROW,                /* -> */
  C_TOK_INCREMENT,            /* ++ */
  C_TOK_DECREMENT,            /* -- */
  C_TOK_LSHIFT,               /* << */
  C_TOK_RSHIFT,               /* >> */
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

  C_TOK_OPEN_BRACKET = '[',
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

void c_lexer_init(jl_lexer_t *self);

#endif /* JL_C_LEXER_H__ */
