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

#include "c_lexer.h"

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
  C_TOK_INLINE,

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
  C_TOK_IDENTIFIER,
  C_TOK_STRING,

  C_TOK_PARAM,
  C_TOK_EMPTY_ARG,
  C_TOK_PREP_NUMBER
};

#define EMPTY {0}
#define TOK(t, s) {(t), {0}, s, sizeof(s)-1}
#define IDN(t, s) {(t), {0}, s, sizeof(s)-1}

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


void c_lexer_stack(jl_lexer_t *self, unsigned n) {
  static const jl_token_t tokens[] = {
    /* 0x00 */
    TOK(C_TOK_END, "$"),
    IDN(C_TOK_AUTO, "auto"),
    IDN(C_TOK_BREAK, "break"),
    IDN(C_TOK_CASE, "case"),
    IDN(C_TOK_CHAR, "char"),
    IDN(C_TOK_CONST, "const"),
    IDN(C_TOK_CONTINUE, "continue"),
    IDN(C_TOK_DEFAULT, "default"),

    /* 0x08 */
    IDN(C_TOK_DO, "do"),
    IDN(C_TOK_DOUBLE, "double"),
    TOK(C_TOK_EOL, "\n"),
    IDN(C_TOK_ELSE, "else"),
    IDN(C_TOK_ENUM, "enum"),
    IDN(C_TOK_EXTERN, "extern"),
    IDN(C_TOK_FLOAT, "float"),
    IDN(C_TOK_FOR, "for"),

    /* 0x10 */
    IDN(C_TOK_GOTO, "goto"),
    IDN(C_TOK_IF, "if"),
    IDN(C_TOK_INT, "int"),
    IDN(C_TOK_LONG, "long"),
    IDN(C_TOK_REGISTER, "register"),
    IDN(C_TOK_RETURN, "return"),
    IDN(C_TOK_SHORT, "short"),
    IDN(C_TOK_SIGNED, "signed"),

    /* 0x18 */
    IDN(C_TOK_SIZEOF, "sizeof"),
    IDN(C_TOK_STATIC, "static"),
    IDN(C_TOK_STRUCT, "struct"),
    IDN(C_TOK_SWITCH, "switch"),
    IDN(C_TOK_TYPEDEF, "typedef"),
    IDN(C_TOK_UNION, "union"),
    IDN(C_TOK_UNSIGNED, "unsigned"),
    IDN(C_TOK_VOID, "void"),

    /* 0x20 */
    IDN(C_TOK_INLINE, "inline"),
    TOK(C_TOK_NOT, "!"),
    IDN(C_TOK_VOLATILE, "volatile"),
    TOK(C_TOK_HASH, "#"),
    IDN(C_TOK_WHILE, "while"),
    TOK(C_TOK_MODULO, "%"),
    TOK(C_TOK_AND, "&"),
    TOK(C_TOK_ALIGNOF, "_Alignof"),

    /* 0x28 */
    TOK(C_TOK_OPEN_PAREN, "("),
    TOK(C_TOK_CLOSE_PAREN, ")"),
    TOK(C_TOK_STAR, "*"),
    TOK(C_TOK_PLUS, "+"),
    TOK(C_TOK_COMMA, ","),
    TOK(C_TOK_MINUS, "-"),
    TOK(C_TOK_DOT, "."),
    TOK(C_TOK_SLASH, "/"),

    /* 0x30 */
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,

    /* 0x38 */
    EMPTY,
    EMPTY,
    TOK(C_TOK_COLON, ":"),
    TOK(C_TOK_SEMICOLON, ";"),
    TOK(C_TOK_LT, "<"),
    TOK(C_TOK_ASSIGN, "="),
    TOK(C_TOK_GT, ">"),
    TOK(C_TOK_QUESTION, "?"),

    /* 0x40 */
    TOK(C_TOK_DOTS, "..."),
    TOK(C_TOK_LOGICAL_OR, "||"),
    TOK(C_TOK_LOGICAL_AND, "&&"),
    TOK(C_TOK_LEQ, "<="),
    TOK(C_TOK_GEQ, ">="),
    TOK(C_TOK_EQ, "=="),
    TOK(C_TOK_NEQ, "!="),
    TOK(C_TOK_ARROW, "->"),

    /* 0x48 */
    TOK(C_TOK_INCREMENT, "++"),
    TOK(C_TOK_DECREMENT, "--"),
    TOK(C_TOK_LSHIFT, "<<"),
    TOK(C_TOK_RSHIFT, ">>"),
    TOK(C_TOK_MUL_ASSIGN, "*="),
    TOK(C_TOK_DIV_ASSIGN, "/="),
    TOK(C_TOK_MOD_ASSIGN, "%="),
    TOK(C_TOK_PLUS_ASSIGN, "+="),

    /* 0x50 */
    TOK(C_TOK_MINUS_ASSIGN, "-="),
    TOK(C_TOK_LSHIFT_ASSIGN, "<<="),
    TOK(C_TOK_RSHIFT_ASSIGN, ">>="),
    TOK(C_TOK_AND_ASSIGN, "&="),
    TOK(C_TOK_XOR_ASSIGN, "^="),
    TOK(C_TOK_OR_ASSIGN, "|="),
    TOK(C_TOK_TOKEN_PASTE, "##"),
    EMPTY,

    /* 0x58 */
    EMPTY,
    EMPTY,
    EMPTY,
    TOK(C_TOK_OPEN_BRACKET, "["),
    EMPTY,
    TOK(C_TOK_CLOSE_BRACKET, "]"),
    TOK(C_TOK_XOR, "^"),
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
    {C_TOK_NUMBER},
    {C_TOK_IDENTIFIER},
    {C_TOK_STRING},
    {C_TOK_PARAM},

    /* 0x78 */
    {C_TOK_EMPTY_ARG},
    {C_TOK_PREP_NUMBER},
    EMPTY,
    TOK(C_TOK_OPEN_CURLY, "{"),
    TOK(C_TOK_OR, "|"),
    TOK(C_TOK_CLOSE_CURLY, "}"),
    TOK(C_TOK_NEG, "~"),
    EMPTY
  };
  const char *ptr;
  char s[256];
  jl_token_t token;
  unsigned i;

  ptr = self->buffer + self->loc.position;
  while (n && peek != C_TOK_END) {
    if (peek == '_' || isalpha(peek)) {
      s[i = 0] = *ptr;
      while (next == '_' || isalnum(peek)) {
        s[++i] = peek;
      }
      s[++i] = '\0';
      switch (s[0]) {
        case 'a':
          if (M(3, 1, 'u', 't', 'o')) {
            token = tokens[C_TOK_AUTO];
            goto push_token;
          }
          break;
        case 'b':
          if (M(4, 1, 'r', 'e', 'a', 'k')) {
            token = tokens[C_TOK_BREAK];
            goto push_token;
          }
          break;
        case 'c':
          if (M(3, 1, 'a', 's', 'e')) {
            token = tokens[C_TOK_CASE];
            goto push_token;
          }
          if (M(3, 1, 'h', 'a', 'r')) {
            token = tokens[C_TOK_CHAR];
            goto push_token;
          }
          if (s[1] == 'o' && s[2] == 'n') {
            if (M(2, 3, 's', 't')) {
              token = tokens[C_TOK_CONST];
              goto push_token;
            }
            if (M(5, 3, 't', 'i', 'n', 'u', 'e')) {
              token = tokens[C_TOK_CONTINUE];
              goto push_token;
            }
          }
        default:
          token = tokens[C_TOK_IDENTIFIER];
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
      token = tokens[C_TOK_PREP_NUMBER];
      push_string;
    } else {
      next;
    }
  }
  if (n) {
    token = tokens[C_TOK_END];
    token.loc = self->loc;
    jl_vector_push(self->token_stack, token);
  }
}
