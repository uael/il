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
  JAY_TOK_INLINE,
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

  JAY_TOK_NOT = '!',
  JAY_TOK_VOLATILE = JAY_TOK_NOT + 1,
  JAY_TOK_HASH = '#',
  JAY_TOK_WHILE = JAY_TOK_HASH + 1,
  JAY_TOK_MODULO = '%',
  JAY_TOK_AND = '&',
  JAY_TOK_ALIGNOF = JAY_TOK_AND + 1,

  JAY_TOK_OPEN_PAREN = '(',
  JAY_TOK_CLOSE_PAREN = ')',
  JAY_TOK_STAR = '*',
  JAY_TOK_PLUS = '+',
  JAY_TOK_COMMA = ',',
  JAY_TOK_MINUS = '-',
  JAY_TOK_DOT = '.',
  JAY_TOK_SLASH = '/',
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
#define SYNTX(t, s) {(t), {0}, s, sizeof(s)-1, false}
#define KEYWD(t, s) {(t), {0}, s, sizeof(s)-1, true}

#define peek *ptr
#define peekn(n) ptr[n]
#define next (self->loc.colno++, self->loc.position++, *++ptr)
#define set_loc token.loc = self->loc
#define set_s do { \
    token.length = i; \
    token.loc.colno -= i; \
    token.loc.position -= i; \
    token.s = xstrndup(s, i); \
  } while (false)
#define push_token do { \
    jl_vector_push(self->queue, token); \
    n--; \
  } while (false)

#define ML(l, n, ...) i == (l+n) && M_ ## l(n, __VA_ARGS__)
#define M(l, n, ...) M_ ## l(n, __VA_ARGS__)
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
  static const jl_token_t tokens[] = {
    /* 0x00 */
    SYNTX(JAY_TOK_END, "$"),
    KEYWD(JAY_TOK_AUTO, "auto"),
    KEYWD(JAY_TOK_BREAK, "break"),
    KEYWD(JAY_TOK_CASE, "case"),
    KEYWD(JAY_TOK_CHAR, "char"),
    KEYWD(JAY_TOK_CONST, "const"),
    KEYWD(JAY_TOK_CONTINUE, "continue"),
    KEYWD(JAY_TOK_DEFAULT, "default"),

    /* 0x08 */
    KEYWD(JAY_TOK_DO, "do"),
    KEYWD(JAY_TOK_DOUBLE, "double"),
    SYNTX(JAY_TOK_EOL, "\n"),
    KEYWD(JAY_TOK_ELSE, "else"),
    KEYWD(JAY_TOK_ENUM, "enum"),
    KEYWD(JAY_TOK_EXTERN, "extern"),
    KEYWD(JAY_TOK_FLOAT, "float"),
    KEYWD(JAY_TOK_FOR, "for"),

    /* 0x10 */
    KEYWD(JAY_TOK_GOTO, "goto"),
    KEYWD(JAY_TOK_IF, "if"),
    KEYWD(JAY_TOK_INLINE, "inline"),
    KEYWD(JAY_TOK_INT, "int"),
    KEYWD(JAY_TOK_LONG, "long"),
    KEYWD(JAY_TOK_REGISTER, "register"),
    KEYWD(JAY_TOK_RETURN, "return"),
    KEYWD(JAY_TOK_SHORT, "short"),

    /* 0x18 */
    KEYWD(JAY_TOK_SIGNED, "signed"),
    KEYWD(JAY_TOK_SIZEOF, "sizeof"),
    KEYWD(JAY_TOK_STATIC, "static"),
    KEYWD(JAY_TOK_STRUCT, "struct"),
    KEYWD(JAY_TOK_SWITCH, "switch"),
    KEYWD(JAY_TOK_TYPEDEF, "typedef"),
    KEYWD(JAY_TOK_UNION, "union"),
    KEYWD(JAY_TOK_UNSIGNED, "unsigned"),

    /* 0x20 */
    KEYWD(JAY_TOK_VOID, "void"),
    SYNTX(JAY_TOK_NOT, "!"),
    KEYWD(JAY_TOK_VOLATILE, "volatile"),
    SYNTX(JAY_TOK_HASH, "#"),
    KEYWD(JAY_TOK_WHILE, "while"),
    SYNTX(JAY_TOK_MODULO, "%"),
    SYNTX(JAY_TOK_AND, "&"),
    SYNTX(JAY_TOK_ALIGNOF, "_Alignof"),

    /* 0x28 */
    SYNTX(JAY_TOK_OPEN_PAREN, "("),
    SYNTX(JAY_TOK_CLOSE_PAREN, ")"),
    SYNTX(JAY_TOK_STAR, "*"),
    SYNTX(JAY_TOK_PLUS, "+"),
    SYNTX(JAY_TOK_COMMA, ","),
    SYNTX(JAY_TOK_MINUS, "-"),
    SYNTX(JAY_TOK_DOT, "."),
    SYNTX(JAY_TOK_SLASH, "/"),

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
    SYNTX(JAY_TOK_COLON, ":"),
    SYNTX(JAY_TOK_SEMICOLON, ";"),
    SYNTX(JAY_TOK_LT, "<"),
    SYNTX(JAY_TOK_ASSIGN, "="),
    SYNTX(JAY_TOK_GT, ">"),
    SYNTX(JAY_TOK_QUESTION, "?"),

    /* 0x40 */
    SYNTX(JAY_TOK_DOTS, "..."),
    SYNTX(JAY_TOK_LOGICAL_OR, "||"),
    SYNTX(JAY_TOK_LOGICAL_AND, "&&"),
    SYNTX(JAY_TOK_LEQ, "<="),
    SYNTX(JAY_TOK_GEQ, ">="),
    SYNTX(JAY_TOK_EQ, "=="),
    SYNTX(JAY_TOK_NEQ, "!="),
    SYNTX(JAY_TOK_ARROW, "->"),

    /* 0x48 */
    SYNTX(JAY_TOK_INCREMENT, "++"),
    SYNTX(JAY_TOK_DECREMENT, "--"),
    SYNTX(JAY_TOK_LSHIFT, "<<"),
    SYNTX(JAY_TOK_RSHIFT, ">>"),
    SYNTX(JAY_TOK_MUL_ASSIGN, "*="),
    SYNTX(JAY_TOK_DIV_ASSIGN, "/="),
    SYNTX(JAY_TOK_MOD_ASSIGN, "%="),
    SYNTX(JAY_TOK_PLUS_ASSIGN, "+="),

    /* 0x50 */
    SYNTX(JAY_TOK_MINUS_ASSIGN, "-="),
    SYNTX(JAY_TOK_LSHIFT_ASSIGN, "<<="),
    SYNTX(JAY_TOK_RSHIFT_ASSIGN, ">>="),
    SYNTX(JAY_TOK_AND_ASSIGN, "&="),
    SYNTX(JAY_TOK_XOR_ASSIGN, "^="),
    SYNTX(JAY_TOK_OR_ASSIGN, "|="),
    SYNTX(JAY_TOK_TOKEN_PASTE, "##"),
    EMPTY,

    /* 0x58 */
    EMPTY,
    EMPTY,
    EMPTY,
    SYNTX(JAY_TOK_OPEN_BRACKET, "["),
    EMPTY,
    SYNTX(JAY_TOK_CLOSE_BRACKET, "]"),
    SYNTX(JAY_TOK_XOR, "^"),
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
    {JAY_TOK_IDENTIFIER},
    {JAY_TOK_STRING},
    {JAY_TOK_PARAM},

    /* 0x78 */
    {JAY_TOK_EMPTY_ARG},
    {JAY_TOK_PREP_NUMBER},
    EMPTY,
    SYNTX(JAY_TOK_OPEN_CURLY, "{"),
    SYNTX(JAY_TOK_OR, "|"),
    SYNTX(JAY_TOK_CLOSE_CURLY, "}"),
    SYNTX(JAY_TOK_NEG, "~"),
    EMPTY
  };
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
          if (ML(3, 1, 'u', 't', 'o')) {
            token = tokens[JAY_TOK_AUTO];
            goto lbl_push_token;
          }
          break;
        case 'b':
          if (ML(4, 1, 'r', 'e', 'a', 'k')) {
            token = tokens[JAY_TOK_BREAK];
            goto lbl_push_token;
          }
          break;
        case 'c':
          if (ML(3, 1, 'a', 's', 'e')) {
            token = tokens[JAY_TOK_CASE];
            goto lbl_push_token;
          }
          if (ML(3, 1, 'h', 'a', 'r')) {
            token = tokens[JAY_TOK_CHAR];
            goto lbl_push_token;
          }
          if (i > 4 && peekn(1) == 'o' && peekn(2) == 'n') {
            if (ML(2, 3, 's', 't')) {
              token = tokens[JAY_TOK_CONST];
              goto lbl_push_token;
            }
            if (ML(5, 3, 't', 'i', 'n', 'u', 'e')) {
              token = tokens[JAY_TOK_CONTINUE];
              goto lbl_push_token;
            }
          }
        case 'd':
          if (ML(6, 1, 'e', 'f', 'a', 'u', 'l', 't')) {
            token = tokens[JAY_TOK_DEFAULT];
            goto lbl_push_token;
          }
          if (i > 1 && peekn(1) == 'o') {
            if (i == 2) {
              token = tokens[JAY_TOK_DO];
              goto lbl_push_token;
            }
            if (ML(4, 2, 'u', 'b', 'l', 'e')) {
              token = tokens[JAY_TOK_DOUBLE];
              goto lbl_push_token;
            }
          }
          break;
        case 'e':
          if (ML(3, 1, 'l', 's', 'e')) {
            token = tokens[JAY_TOK_ELSE];
            goto lbl_push_token;
          }
          if (ML(3, 1, 'n', 'u', 'm')) {
            token = tokens[JAY_TOK_ENUM];
            goto lbl_push_token;
          }
          if (ML(5, 1, 'x', 't', 'e', 'r', 'n')) {
            token = tokens[JAY_TOK_EXTERN];
            goto lbl_push_token;
          }
          break;
        case 'f':
          if (ML(4, 1, 'l', 'o', 'a', 't')) {
            token = tokens[JAY_TOK_FLOAT];
            goto lbl_push_token;
          }
          if (ML(2, 1, 'o', 'r')) {
            token = tokens[JAY_TOK_FOR];
            goto lbl_push_token;
          }
          break;
        case 'g':
          if (ML(3, 1, 'o', 't', 'o')) {
            token = tokens[JAY_TOK_GOTO];
            goto lbl_push_token;
          }
          break;
        case 'i':
          if (ML(1, 1, 'f')) {
            token = tokens[JAY_TOK_IF];
            goto lbl_push_token;
          }
          if (ML(5, 1, 'n', 'l', 'i', 'n', 'e')) {
            token = tokens[JAY_TOK_INLINE];
            goto lbl_push_token;
          }
          if (ML(2, 1, 'n', 't')) {
            token = tokens[JAY_TOK_INT];
            goto lbl_push_token;
          }
          break;
        case 'l':
          if (ML(3, 1, 'o', 'n', 'g')) {
            token = tokens[JAY_TOK_LONG];
            goto lbl_push_token;
          }
        case 'r':
          if (i > 4 && peekn(1) == 'e') {
            if (ML(6, 2, 'g', 'i', 's', 't', 'e', 'r')) {
              token = tokens[JAY_TOK_REGISTER];
              goto lbl_push_token;
            }
            if (ML(4, 2, 't', 'u', 'r', 'n')) {
              token = tokens[JAY_TOK_RETURN];
              goto lbl_push_token;
            }
          }
          break;
        case 's':
          if (ML(4, 1, 'h', 'o', 'r', 't')) {
            token = tokens[JAY_TOK_SHORT];
            goto lbl_push_token;
          }
          if (i == 6) {
            switch (peekn(1)) {
              case 'i':
                if (M(4, 2, 'g', 'n', 'e', 'd')) {
                  token = tokens[JAY_TOK_SIGNED];
                  goto lbl_push_token;
                }
                if (M(4, 2, 'z', 'e', 'o', 'f')) {
                  token = tokens[JAY_TOK_SIZEOF];
                  goto lbl_push_token;
                }
                break;
              case 't':
                if (M(4, 2, 'a', 't', 'i', 'c')) {
                  token = tokens[JAY_TOK_STATIC];
                  goto lbl_push_token;
                }
                if (M(4, 2, 'r', 'u', 'c', 't')) {
                  token = tokens[JAY_TOK_STRUCT];
                  goto lbl_push_token;
                }
                break;
              case 'w':
                if (M(4, 2, 'i', 't', 'c', 'h')) {
                  token = tokens[JAY_TOK_SWITCH];
                  goto lbl_push_token;
                }
                break;
            }
          }
          break;
        case 't':
          if (ML(6, 1, 'y', 'p', 'e', 'd', 'e', 'f')) {
            token = tokens[JAY_TOK_TYPEDEF];
            goto lbl_push_token;
          }
          break;
        case 'u':
          if (i > 4 && peekn(1) == 'n') {
            if (M(3, 2, 'i', 'o', 'n')) {
              token = tokens[JAY_TOK_UNION];
              goto lbl_push_token;
            }
            if (M(6, 2, 's', 'i', 'g', 'n', 'e', 'd')) {
              token = tokens[JAY_TOK_UNSIGNED];
              goto lbl_push_token;
            }
          }
          break;
        case 'v':
          if (i > 3 && peekn(1) == 'o') {
            if (M(2, 2, 'i', 'd')) {
              token = tokens[JAY_TOK_VOID];
              goto lbl_push_token;
            }
            if (M(6, 2, 'l', 'a', 't', 'i', 'l', 'e')) {
              token = tokens[JAY_TOK_VOLATILE];
              goto lbl_push_token;
            }
          }
          break;
        case 'w':
          if (M(4, 1, 'h', 'i', 'l', 'e')) {
            token = tokens[JAY_TOK_WHILE];
            goto lbl_push_token;
          }
          break;
        default:
          token = tokens[JAY_TOK_IDENTIFIER];
          set_loc;
          set_s;
          push_token;
          break;
        lbl_push_token:
          set_loc;
          push_token;
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
      set_loc;
      set_s;
      push_token;
    } else {
      next;
    }
  }
  if (n) {
    token = tokens[JAY_TOK_END];
    token.loc = self->loc;
    jl_vector_push(self->queue, token);
  }
}
