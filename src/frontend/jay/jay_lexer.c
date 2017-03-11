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

#include <stdlib.h>
#include <ctype.h>
#include <adt/xmalloc.h>

#include "jay_lexer.h"

enum {
  JAY_TOK_END = 0,
  JAY_TOK_AUTO = JAY_TOK_END + 1,
  JAY_TOK_BREAK,
  JAY_TOK_CASE,
  JAY_TOK_CHAR,
  JAY_TOK_CONST,
  JAY_TOK_CONTINUE,
  JAY_TOK_DEFAULT,
  JAY_TOK_DO,
  JAY_TOK_DOUBLE,
  JAY_TOK_EOL = '\n',
  JAY_TOK_ELSE = JAY_TOK_EOL + 1,
  JAY_TOK_ENUM,
  JAY_TOK_EXTERN,
  JAY_TOK_FLOAT,
  JAY_TOK_FOR,
  JAY_TOK_GOTO,
  JAY_TOK_IF,
  JAY_TOK_INT,
  JAY_TOK_LONG,
  JAY_TOK_REGISTER,
  JAY_TOK_RETURN,
  JAY_TOK_SHORT,
  JAY_TOK_SIGNED,
  JAY_TOK_SIZEOF,
  JAY_TOK_STATIC,
  JAY_TOK_STRUCT,
  JAY_TOK_SWITCH,
  JAY_TOK_TYPEDEF,
  JAY_TOK_UNION,
  JAY_TOK_UNSIGNED,
  JAY_TOK_VOID,
  JAY_TOK_INLINE,

  JAY_TOK_NOT = '!',
  JAY_TOK_VOLATILE = JAY_TOK_NOT + 1,
  JAY_TOK_HASH = '#',
  JAY_TOK_WHILE = JAY_TOK_HASH + 1,
  JAY_TOK_MODULO = '%',
  JAY_TOK_AND = '&',
  JAY_TOK_INCLUDE = JAY_TOK_AND + 1,

  JAY_TOK_OPEN_PAREN = '(',
  JAY_TOK_CLOSE_PAREN = ')',
  JAY_TOK_STAR = '*',
  JAY_TOK_PLUS = '+',
  JAY_TOK_COMMA = ',',
  JAY_TOK_MINUS = '-',
  JAY_TOK_DOT = '.',
  JAY_TOK_SLASH = '/',
  JAY_TOK_USE = JAY_TOK_SLASH + 1,
  JAY_TOK_DCOLON, /* :: */
  JAY_TOK_DARROW, /* => */
  JAY_TOK_DQUESTION, /* ?? */
  JAY_TOK_NAMESPACE,

  JAY_TOK_COLON = ':',
  JAY_TOK_SEMICOLON = ';',
  JAY_TOK_LT = '<',
  JAY_TOK_ASSIGN = '=',
  JAY_TOK_GT = '>',
  JAY_TOK_QUESTION = '?',
  JAY_TOK_DOTS = JAY_TOK_QUESTION + 1,  /* ... */
  JAY_TOK_LOGICAL_OR,           /* || */
  JAY_TOK_LOGICAL_AND,          /* && */
  JAY_TOK_LEQ,                  /* <= */
  JAY_TOK_GEQ,                  /* >= */
  JAY_TOK_EQ,                   /* == */
  JAY_TOK_NEQ,                  /* != */
  JAY_TOK_ARROW,                /* -> */
  JAY_TOK_INCREMENT,            /* ++ */
  JAY_TOK_DECREMENT,            /* -- */
  JAY_TOK_LSHIFT,               /* << */
  JAY_TOK_RSHIFT,               /* >> */
  JAY_TOK_MUL_ASSIGN,           /* *= */
  JAY_TOK_DIV_ASSIGN,           /* /= */
  JAY_TOK_MOD_ASSIGN,           /* %= */
  JAY_TOK_PLUS_ASSIGN,          /* += */
  JAY_TOK_MINUS_ASSIGN,         /* -= */
  JAY_TOK_LSHIFT_ASSIGN,        /* <<= */
  JAY_TOK_RSHIFT_ASSIGN,        /* >>= */
  JAY_TOK_AND_ASSIGN,           /* &= */
  JAY_TOK_XOR_ASSIGN,           /* ^= */
  JAY_TOK_OR_ASSIGN,            /* |= */
  JAY_TOK_TOKEN_PASTE,          /* ## */

  JAY_TOK_OPEN_BRACKET = '[',
  JAY_TOK_CLOSE_BRACKET = ']',
  JAY_TOK_XOR = '^',
  JAY_TOK_OPEN_CURLY = '{',
  JAY_TOK_OR = '|',
  JAY_TOK_CLOSE_CURLY = '}',
  JAY_TOK_NEG = '~',

  JAY_TOK_NUMBER = 116,
  JAY_TOK_IDENTIFIER,
  JAY_TOK_STRING,

  JAY_TOK_PARAM,
  JAY_TOK_EMPTY_ARG,
  JAY_TOK_PREP_NUMBER
};

#define EMPTY {0}
#define TOK(t, s) {(t), {0}, s, sizeof(s)-1}
#define IDN(t, s) {(t), {0}, s, sizeof(s)-1}

const jl_token_t tokens[] = {
  /* 0x00 */
  TOK(JAY_TOK_END, "$"),
  IDN(JAY_TOK_AUTO, "auto"),
  IDN(JAY_TOK_BREAK, "break"),
  IDN(JAY_TOK_CASE, "case"),
  IDN(JAY_TOK_CHAR, "char"),
  IDN(JAY_TOK_CONST, "const"),
  IDN(JAY_TOK_CONTINUE, "continue"),
  IDN(JAY_TOK_DEFAULT, "default"),

  /* 0x08 */
  IDN(JAY_TOK_DO, "do"),
  IDN(JAY_TOK_DOUBLE, "double"),
  TOK(JAY_TOK_EOL, "\n"),
  IDN(JAY_TOK_ELSE, "else"),
  IDN(JAY_TOK_ENUM, "enum"),
  IDN(JAY_TOK_EXTERN, "extern"),
  IDN(JAY_TOK_FLOAT, "float"),
  IDN(JAY_TOK_FOR, "for"),

  /* 0x10 */
  IDN(JAY_TOK_GOTO, "goto"),
  IDN(JAY_TOK_IF, "if"),
  IDN(JAY_TOK_INT, "int"),
  IDN(JAY_TOK_LONG, "long"),
  IDN(JAY_TOK_REGISTER, "register"),
  IDN(JAY_TOK_RETURN, "return"),
  IDN(JAY_TOK_SHORT, "short"),
  IDN(JAY_TOK_SIGNED, "signed"),

  /* 0x18 */
  IDN(JAY_TOK_SIZEOF, "sizeof"),
  IDN(JAY_TOK_STATIC, "static"),
  IDN(JAY_TOK_STRUCT, "struct"),
  IDN(JAY_TOK_SWITCH, "switch"),
  IDN(JAY_TOK_TYPEDEF, "typedef"),
  IDN(JAY_TOK_UNION, "union"),
  IDN(JAY_TOK_UNSIGNED, "unsigned"),
  IDN(JAY_TOK_VOID, "void"),

  /* 0x20 */
  IDN(JAY_TOK_INLINE, "inline"),
  TOK(JAY_TOK_NOT, "!"),
  IDN(JAY_TOK_VOLATILE, "volatile"),
  TOK(JAY_TOK_HASH, "#"),
  IDN(JAY_TOK_WHILE, "while"),
  TOK(JAY_TOK_MODULO, "%"),
  TOK(JAY_TOK_AND, "&"),
  IDN(JAY_TOK_INCLUDE, "include"),

  /* 0x28 */
  TOK(JAY_TOK_OPEN_PAREN, "("),
  TOK(JAY_TOK_CLOSE_PAREN, ")"),
  TOK(JAY_TOK_STAR, "*"),
  TOK(JAY_TOK_PLUS, "+"),
  TOK(JAY_TOK_COMMA, ","),
  TOK(JAY_TOK_MINUS, "-"),
  TOK(JAY_TOK_DOT, "."),
  TOK(JAY_TOK_SLASH, "/"),

  /* 0x30 */
  IDN(JAY_TOK_USE, "use"),
  TOK(JAY_TOK_DCOLON, "::"),
  TOK(JAY_TOK_DARROW, "=>"),
  TOK(JAY_TOK_DQUESTION, "??"),
  IDN(JAY_TOK_NAMESPACE, "namespace"),
  EMPTY,
  EMPTY,
  EMPTY,

  /* 0x38 */
  EMPTY,
  EMPTY,
  TOK(JAY_TOK_COLON, ":"),
  TOK(JAY_TOK_SEMICOLON, ";"),
  TOK(JAY_TOK_LT, "<"),
  TOK(JAY_TOK_ASSIGN, "="),
  TOK(JAY_TOK_GT, ">"),
  TOK(JAY_TOK_QUESTION, "?"),

  /* 0x40 */
  TOK(JAY_TOK_DOTS, "..."),
  TOK(JAY_TOK_LOGICAL_OR, "||"),
  TOK(JAY_TOK_LOGICAL_AND, "&&"),
  TOK(JAY_TOK_LEQ, "<="),
  TOK(JAY_TOK_GEQ, ">="),
  TOK(JAY_TOK_EQ, "=="),
  TOK(JAY_TOK_NEQ, "!="),
  TOK(JAY_TOK_ARROW, "->"),

  /* 0x48 */
  TOK(JAY_TOK_INCREMENT, "++"),
  TOK(JAY_TOK_DECREMENT, "--"),
  TOK(JAY_TOK_LSHIFT, "<<"),
  TOK(JAY_TOK_RSHIFT, ">>"),
  TOK(JAY_TOK_MUL_ASSIGN, "*="),
  TOK(JAY_TOK_DIV_ASSIGN, "/="),
  TOK(JAY_TOK_MOD_ASSIGN, "%="),
  TOK(JAY_TOK_PLUS_ASSIGN, "+="),

  /* 0x50 */
  TOK(JAY_TOK_MINUS_ASSIGN, "-="),
  TOK(JAY_TOK_LSHIFT_ASSIGN, "<<="),
  TOK(JAY_TOK_RSHIFT_ASSIGN, ">>="),
  TOK(JAY_TOK_AND_ASSIGN, "&="),
  TOK(JAY_TOK_XOR_ASSIGN, "^="),
  TOK(JAY_TOK_OR_ASSIGN, "|="),
  TOK(JAY_TOK_TOKEN_PASTE, "##"),
  EMPTY,

  /* 0x58 */
  EMPTY,
  EMPTY,
  EMPTY,
  TOK(JAY_TOK_OPEN_BRACKET, "["),
  EMPTY,
  TOK(JAY_TOK_CLOSE_BRACKET, "]"),
  TOK(JAY_TOK_XOR, "^"),
  EMPTY,

  /* 0x60 */
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,

  /* 0x68 */
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,

  /* 0x70 */
  EMPTY,
  EMPTY,
  EMPTY,
  EMPTY,
  {JAY_TOK_NUMBER},
  {JAY_TOK_IDENTIFIER, 1, 1},
  {JAY_TOK_STRING},
  {JAY_TOK_PARAM},

  /* 0x78 */
  {JAY_TOK_EMPTY_ARG},
  {JAY_TOK_PREP_NUMBER},
  EMPTY,
  TOK(JAY_TOK_OPEN_CURLY, "{"),
  TOK(JAY_TOK_OR, "|"),
  TOK(JAY_TOK_CLOSE_CURLY, "}"),
  TOK(JAY_TOK_NEG, "~"),
  EMPTY
};

#define peek *ptr
#define peekn(n) ptr[n]
#define next (self->loc.colno++, self->loc.position++, *++ptr)
#define push_string do { \
    token.loc = self->loc; \
    token.length = i; \
    token.loc.colno -= i; \
    token.loc.position -= i; \
    token.s = xstrndup(s, i); \
    jl_vector_push(self->token_stack, token); \
    n--; \
  } while (false)

#define M(l, n, ...) i == (l+n) && M_ ## l(n, __VA_ARGS__)
#define M_(n, i, c) s[i + n] == (c)
#define M_1(n, a) M_(n, 0, a)
#define M_2(n, a, b) M_1(n, a) && M_(n, 1, b)
#define M_3(n, a, b, c) M_2(n, a, b) && M_(n, 2, c)
#define M_4(n, a, b, c, d) M_3(n, a, b, c) && M_(n, 3, d)
#define M_5(n, a, b, c, d, e) M_4(n, a, b, c, d) && M_(n, 4, e)
#define M_6(n, a, b, c, d, e, f) M_5(n, a, b, c, d, e) && M_(n, 5, f)
#define M_7(n, a, b, c, d, e, f, g) M_6(n, a, b, c, d, e, f) && M_(n, 6, g)
#define M_8(n, a, b, c, d, e, f, g, h) M_7(n, a, b, c, d, e, f, g) && M_(n, 7, h)


void jay_lexer_stack(jl_lexer_t *self, unsigned n) {
  const char *ptr;
  char s[256];
  jl_token_t token;
  unsigned i;

  ptr = self->buffer + self->loc.position;
  while (n && peek != JAY_TOK_END) {
    if (peek == '_' || isalpha(peek)) {
      s[i = 0] = *ptr;
      while (next == '_' || isalnum(peek)) {
        s[++i] = peek;
      }
      s[++i] = '\0';
      switch (s[0]) {
        case 'a':
          if (M(3, 1, 'u', 't', 'o')) {
            token = tokens[JAY_TOK_AUTO];
            goto push_token;
          }
          break;
        case 'b':
          if (M(4, 1, 'r', 'e', 'a', 'k')) {
            token = tokens[JAY_TOK_BREAK];
            goto push_token;
          }
          break;
        case 'c':
          if (M(3, 1, 'a', 's', 'e')) {
            token = tokens[JAY_TOK_CASE];
            goto push_token;
          }
          if (M(3, 1, 'h', 'a', 'r')) {
            token = tokens[JAY_TOK_CHAR];
            goto push_token;
          }
          if (s[1] == 'o' && s[2] == 'n') {
            if (M(2, 3, 's', 't')) {
              token = tokens[JAY_TOK_CONST];
              goto push_token;
            }
            if (M(5, 3, 't', 'i', 'n', 'u', 'e')) {
              token = tokens[JAY_TOK_CONTINUE];
              goto push_token;
            }
          }
        default:
          token = tokens[JAY_TOK_IDENTIFIER];
        push_token:
          push_string;
          break;
      }
    } else if (isdigit(peek) || (peek == '.' && isdigit(peekn(1)))) {
      s[i = 0] = peek;
      while (true) {
        if (isdigit(next) || peek == '.' || peek == '_') {
          s[++i] = peek;
        } else if (isalpha(peek)) {
          if ((tolower(peek) == 'e' || (tolower(peek) == 'p'))
            && (peekn(1) == '+' || peekn(1) == '-')) {
            s[++i] = peek;
            next;
          }
          s[++i] = peek;
        } else {
          s[++i] = '\0';
          break;
        }
      }
      token = tokens[JAY_TOK_PREP_NUMBER];
      push_string;
    } else {
      next;
    }
  }
  if (n) {
    token = tokens[JAY_TOK_END];
    token.loc = self->loc;
    jl_vector_push(self->token_stack, token);
  }
}
