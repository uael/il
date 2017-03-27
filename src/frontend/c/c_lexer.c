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
#include <adt/string.h>

#include "c_lexer.h"

#include "c_pp.h"
#include "compiler.h"

static void c_lexer_enqueue(jl_lexer_t *self, unsigned n);

void c_lexer_init(jl_lexer_t *self) {
  jl_lexer_event_t event;

  self->enqueue = c_lexer_enqueue;
  event = (jl_lexer_event_t) {
    .kind = JL_LEXER_EVENT_ON_PUSH,
    .callback = c_pp_op_push_callback,
    .dtor = c_pp_op_push_dtor,
    .data = xmalloc(sizeof(c_pp_t))
  };
  c_pp_init(event.data);
  jl_lexer_attach(self, event);
}

#define EMPTY {0}
#define SYNTX(t, s) {(t), {0}, s, sizeof(s)-1, JL_TOKEN_SYNTAX}
#define KEYWD(t, s) {(t), {0}, s, sizeof(s)-1, JL_TOKEN_KEYWORD}

#define peek *(self->buffer + self->loc.position)
#define peekn(n) (self->buffer + self->loc.position)[n]
#define next (self->loc.colno++, self->loc.position++, peek)
#define nextn(n) (self->loc.colno+=n, self->loc.position+=n, peek)

#define set_loc token.loc = self->loc
#define set_s do { \
    token.length = i; \
    token.loc.colno -= i; \
    token.loc.position -= i; \
    token.u.s = jl_strndup(self->fe->compiler, s, i); \
  } while (false)
#define push_token do { \
    if (jl_lexer_push(self, token)) n--; \
  } while (false)
#define push(t) do { \
    token = tokens[t]; \
    set_loc; \
    nextn(token.length); \
    push_token; \
  } while (false)

#define __M(n, i, c) s[i + n] == (c)
#define __M1(n, a) __M(n, 0, a)
#define __M2(n, a, b) __M1(n, a) && __M(n, 1, b)
#define __M3(n, a, b, c) __M2(n, a, b) && __M(n, 2, c)
#define __M4(n, a, b, c, d) __M3(n, a, b, c) && __M(n, 3, d)
#define __M5(n, a, b, c, d, e) __M4(n, a, b, c, d) && __M(n, 4, e)
#define __M6(n, a, b, c, d, e, f) __M5(n, a, b, c, d, e) && __M(n, 5, f)
#define __M7(n, a, b, c, d, e, f, g) __M6(n, a, b, c, d, e, f) && __M(n, 6, g)
#define __M8(n, a, b, c, d, e, f, g, h) __M7(n, a, b, c, d, e, f, g) && __M(n, 7, h)
#define M1(n, a) i == (1+n) && __M1(n, a)
#define M2(n, a, b) i == (2+n) && __M2(n, a, b)
#define M3(n, a, b, c) i == (3+n) && __M3(n, a, b, c)
#define M4(n, a, b, c, d) i == (4+n) && __M4(n, a, b, c, d)
#define M5(n, a, b, c, d, e) i == (5+n) && __M5(n, a, b, c, d, e)
#define M6(n, a, b, c, d, e, f) i == (6+n) && __M6(n, a, b, c, d, e, f)
#define M7(n, a, b, c, d, e, f, g) i == (7+n) && __M7(n, a, b, c, d, e, f, g)
#define M8(n, a, b, c, d, e, f, g, h) i == (8+n) && __M8(n, a, b, c, d, e, f, g, h)


static void c_lexer_enqueue(jl_lexer_t *self, unsigned n) {
  static const jl_token_t tokens[] = {
    /* 0x00 */
    SYNTX(C_TOK_END, "\0"),
    KEYWD(C_TOK_AUTO, "auto"),
    KEYWD(C_TOK_BREAK, "break"),
    KEYWD(C_TOK_CASE, "case"),
    KEYWD(C_TOK_CHAR, "char"),
    KEYWD(C_TOK_CONST, "const"),
    KEYWD(C_TOK_CONTINUE, "continue"),
    KEYWD(C_TOK_DEFAULT, "default"),

    /* 0x08 */
    KEYWD(C_TOK_DO, "do"),
    KEYWD(C_TOK_DOUBLE, "double"),
    SYNTX(C_TOK_EOL, "\n"),
    KEYWD(C_TOK_ELSE, "else"),
    KEYWD(C_TOK_ENUM, "enum"),
    KEYWD(C_TOK_EXTERN, "extern"),
    KEYWD(C_TOK_FLOAT, "float"),
    KEYWD(C_TOK_FOR, "for"),

    /* 0x10 */
    KEYWD(C_TOK_GOTO, "goto"),
    KEYWD(C_TOK_IF, "if"),
    KEYWD(C_TOK_INLINE, "inline"),
    KEYWD(C_TOK_INT, "int"),
    KEYWD(C_TOK_LONG, "long"),
    KEYWD(C_TOK_REGISTER, "register"),
    KEYWD(C_TOK_RESTRICT, "restrict"),
    KEYWD(C_TOK_RETURN, "return"),

    /* 0x18 */
    KEYWD(C_TOK_SHORT, "short"),
    KEYWD(C_TOK_SIGNED, "signed"),
    KEYWD(C_TOK_SIZEOF, "sizeof"),
    KEYWD(C_TOK_STATIC, "static"),
    KEYWD(C_TOK_STRUCT, "struct"),
    KEYWD(C_TOK_SWITCH, "switch"),
    KEYWD(C_TOK_TYPEDEF, "typedef"),
    KEYWD(C_TOK_UNION, "union"),

    /* 0x20 */
    KEYWD(C_TOK_UNSIGNED, "unsigned"),
    SYNTX(C_TOK_NOT, "!"),
    KEYWD(C_TOK_VOID, "void"),
    SYNTX(C_TOK_HASH, "#"),
    KEYWD(C_TOK_VOLATILE, "volatile"),
    SYNTX(C_TOK_MODULO, "%"),
    SYNTX(C_TOK_AND, "&"),
    KEYWD(C_TOK_WHILE, "while"),

    /* 0x28 */
    SYNTX(C_TOK_OPEN_PAREN, "("),
    SYNTX(C_TOK_CLOSE_PAREN, ")"),
    SYNTX(C_TOK_STAR, "*"),
    SYNTX(C_TOK_PLUS, "+"),
    SYNTX(C_TOK_COMMA, ","),
    SYNTX(C_TOK_MINUS, "-"),
    SYNTX(C_TOK_DOT, "."),
    SYNTX(C_TOK_SLASH, "/"),

    /* 0x30 */
    SYNTX(C_TOK_ALIGNAS, "_Alignas"),
    SYNTX(C_TOK_ALIGNOF, "_Alignof"),
    KEYWD(C_TOK_ATOMIC, "_Atomic"),
    KEYWD(C_TOK_BOOL, "_Bool"),
    KEYWD(C_TOK_COMPLEX, "_Complex"),
    KEYWD(C_TOK_GENERIC, "_Generic"),
    KEYWD(C_TOK_IMAGINARY, "_Imaginary"),
    KEYWD(C_TOK_NORETURN, "_Noreturn"),

    /* 0x38 */
    KEYWD(C_TOK_STATIC_ASSERT, "_Static_assert"),
    KEYWD(C_TOK_THREAD_LOCAL, "_Thread_local"),
    SYNTX(C_TOK_COLON, ":"),
    SYNTX(C_TOK_SEMICOLON, ";"),
    SYNTX(C_TOK_LT, "<"),
    SYNTX(C_TOK_ASSIGN, "="),
    SYNTX(C_TOK_GT, ">"),
    SYNTX(C_TOK_QUESTION, "?"),

    /* 0x40 */
    SYNTX(C_TOK_DOTS, "..."),
    SYNTX(C_TOK_LOGICAL_OR, "||"),
    SYNTX(C_TOK_LOGICAL_AND, "&&"),
    SYNTX(C_TOK_LEQ, "<="),
    SYNTX(C_TOK_GEQ, ">="),
    SYNTX(C_TOK_EQ, "=="),
    SYNTX(C_TOK_NEQ, "!="),
    SYNTX(C_TOK_ARROW, "->"),

    /* 0x48 */
    SYNTX(C_TOK_INCREMENT, "++"),
    SYNTX(C_TOK_DECREMENT, "--"),
    SYNTX(C_TOK_LSHIFT, "<<"),
    SYNTX(C_TOK_RSHIFT, ">>"),
    SYNTX(C_TOK_MUL_ASSIGN, "*="),
    SYNTX(C_TOK_DIV_ASSIGN, "/="),
    SYNTX(C_TOK_MOD_ASSIGN, "%="),
    SYNTX(C_TOK_PLUS_ASSIGN, "+="),

    /* 0x50 */
    SYNTX(C_TOK_MINUS_ASSIGN, "-="),
    SYNTX(C_TOK_LSHIFT_ASSIGN, "<<="),
    SYNTX(C_TOK_RSHIFT_ASSIGN, ">>="),
    SYNTX(C_TOK_AND_ASSIGN, "&="),
    SYNTX(C_TOK_XOR_ASSIGN, "^="),
    SYNTX(C_TOK_OR_ASSIGN, "|="),
    SYNTX(C_TOK_TOKEN_PASTE, "##"),
    KEYWD(C_TOK_FUNC_NAME, "__func__"),

    /* 0x58 */
    EMPTY,
    EMPTY,
    EMPTY,
    SYNTX(C_TOK_OPEN_BRACKET, "["),
    SYNTX(C_TOK_ANTI_SLASH, "\\"),
    SYNTX(C_TOK_CLOSE_BRACKET, "]"),
    SYNTX(C_TOK_XOR, "^"),
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
    {C_TOK_NUMBER, {0}, NULL, 0, JL_TOKEN_NUMBER},
    {C_TOK_IDENTIFIER, {0}, NULL, 0, JL_TOKEN_IDENTIFIER},
    {C_TOK_STRING, {0}, NULL, 0, JL_TOKEN_STRING_LITERAL},
    EMPTY,

    /* 0x78 */
    EMPTY,
    EMPTY,
    EMPTY,
    SYNTX(C_TOK_OPEN_CURLY, "{"),
    SYNTX(C_TOK_OR, "|"),
    SYNTX(C_TOK_CLOSE_CURLY, "}"),
    SYNTX(C_TOK_NEG, "~"),
    EMPTY
  };
  char s[256];
  jl_token_t token;
  unsigned i;

  while (n && peek != C_TOK_END) {
    if (peek == '_' || isalpha(peek)) {
      s[i = 0] = peek;
      while (next == '_' || isalnum(peek)) {
        s[++i] = peek;
      }
      s[++i] = '\0';
      switch (s[0]) {
        case 'a':
          if (M3(1, 'u', 't', 'o')) {
            token = tokens[C_TOK_AUTO];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'b':
          if (M4(1, 'r', 'e', 'a', 'k')) {
            token = tokens[C_TOK_BREAK];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'c':
          if (M3(1, 'a', 's', 'e')) {
            token = tokens[C_TOK_CASE];
            goto lbl_push_token;
          }
          if (M3(1, 'h', 'a', 'r')) {
            token = tokens[C_TOK_CHAR];
            goto lbl_push_token;
          }
          if (i >= 4 && s[1] == 'o' && s[2] == 'n') {
            if (M2(3, 's', 't')) {
              token = tokens[C_TOK_CONST];
              goto lbl_push_token;
            }
            if (M5(3, 't', 'i', 'n', 'u', 'e')) {
              token = tokens[C_TOK_CONTINUE];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'd':
          if (M6(1, 'e', 'f', 'a', 'u', 'l', 't')) {
            token = tokens[C_TOK_DEFAULT];
            goto lbl_push_token;
          }
          if (i > 1 && s[1] == 'o') {
            if (i == 2) {
              token = tokens[C_TOK_DO];
              goto lbl_push_token;
            }
            if (M4(2, 'u', 'b', 'l', 'e')) {
              token = tokens[C_TOK_DOUBLE];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'e':
          if (M3(1, 'l', 's', 'e')) {
            token = tokens[C_TOK_ELSE];
            goto lbl_push_token;
          }
          if (M3(1, 'n', 'u', 'm')) {
            token = tokens[C_TOK_ENUM];
            goto lbl_push_token;
          }
          if (M5(1, 'x', 't', 'e', 'r', 'n')) {
            token = tokens[C_TOK_EXTERN];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'f':
          if (M4(1, 'l', 'o', 'a', 't')) {
            token = tokens[C_TOK_FLOAT];
            goto lbl_push_token;
          }
          if (M2(1, 'o', 'r')) {
            token = tokens[C_TOK_FOR];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'g':
          if (M3(1, 'o', 't', 'o')) {
            token = tokens[C_TOK_GOTO];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'i':
          if (M1(1, 'f')) {
            token = tokens[C_TOK_IF];
            goto lbl_push_token;
          }
          if (M5(1, 'n', 'l', 'i', 'n', 'e')) {
            token = tokens[C_TOK_INLINE];
            goto lbl_push_token;
          }
          if (M2(1, 'n', 't')) {
            token = tokens[C_TOK_INT];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'l':
          if (M3(1, 'o', 'n', 'g')) {
            token = tokens[C_TOK_LONG];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'r':
          if (i > 4 && s[1] == 'e') {
            if (M6(2, 'g', 'i', 's', 't', 'e', 'r')) {
              token = tokens[C_TOK_REGISTER];
              goto lbl_push_token;
            }
            if (M6(2, 's', 't', 'r', 'i', 'c', 't')) {
              token = tokens[C_TOK_RESTRICT];
              goto lbl_push_token;
            }
            if (M4(2, 't', 'u', 'r', 'n')) {
              token = tokens[C_TOK_RETURN];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 's':
          if (M4(1, 'h', 'o', 'r', 't')) {
            token = tokens[C_TOK_SHORT];
            goto lbl_push_token;
          }
          if (i == 6) {
            switch (s[1]) {
              case 'i':
                if (__M4(2, 'g', 'n', 'e', 'd')) {
                  token = tokens[C_TOK_SIGNED];
                  goto lbl_push_token;
                }
                if (__M4(2, 'z', 'e', 'o', 'f')) {
                  token = tokens[C_TOK_SIZEOF];
                  goto lbl_push_token;
                }
                break;
              case 't':
                if (__M4(2, 'a', 't', 'i', 'c')) {
                  token = tokens[C_TOK_STATIC];
                  goto lbl_push_token;
                }
                if (__M4(2, 'r', 'u', 'c', 't')) {
                  token = tokens[C_TOK_STRUCT];
                  goto lbl_push_token;
                }
                break;
              case 'w':
                if (__M4(2, 'i', 't', 'c', 'h')) {
                  token = tokens[C_TOK_SWITCH];
                  goto lbl_push_token;
                }
                break;
              default:
                break;
            }
          }
          goto lbl_push_ident;
        case 't':
          if (M6(1, 'y', 'p', 'e', 'd', 'e', 'f')) {
            token = tokens[C_TOK_TYPEDEF];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'u':
          if (i > 4 && s[1] == 'n') {
            if (M3(2, 'i', 'o', 'n')) {
              token = tokens[C_TOK_UNION];
              goto lbl_push_token;
            }
            if (M6(2, 's', 'i', 'g', 'n', 'e', 'd')) {
              token = tokens[C_TOK_UNSIGNED];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'v':
          if (i > 3 && s[1] == 'o') {
            if (M2(2, 'i', 'd')) {
              token = tokens[C_TOK_VOID];
              goto lbl_push_token;
            }
            if (M6(2, 'l', 'a', 't', 'i', 'l', 'e')) {
              token = tokens[C_TOK_VOLATILE];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'w':
          if (M4(1, 'h', 'i', 'l', 'e')) {
            token = tokens[C_TOK_WHILE];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case '_':
          switch (s[1]) {
            case 'A':
              if (i == 8 && __M4(2, 'l', 'i', 'g', 'n')) {
                if (__M2(6, 'a', 's')) {
                  token = tokens[C_TOK_ALIGNAS];
                  goto lbl_push_token;
                }
                if (__M2(6, 'o', 'f')) {
                  token = tokens[C_TOK_ALIGNOF];
                  goto lbl_push_token;
                }
              }
              if (M5(2, 't', 'o', 'm', 'i', 'c')) {
                token = tokens[C_TOK_ATOMIC];
                goto lbl_push_token;
              }
              break;
            case 'B':
              if (M3(2, 'o', 'o', 'l')) {
                token = tokens[C_TOK_BOOL];
                goto lbl_push_token;
              }
              break;
            case 'C':
              if (M6(2, 'o', 'm', 'p', 'l', 'e', 'x')) {
                token = tokens[C_TOK_COMPLEX];
                goto lbl_push_token;
              }
              break;
            case 'G':
              if (M6(2, 'e', 'n', 'e', 'r', 'i', 'c')) {
                token = tokens[C_TOK_GENERIC];
                goto lbl_push_token;
              }
              break;
            case 'I':
              if (M8(2, 'm', 'a', 'g', 'i', 'n', 'a', 'r', 'y')) {
                token = tokens[C_TOK_IMAGINARY];
                goto lbl_push_token;
              }
              break;
            case 'N':
              if (M7(2, 'o', 'r', 'e', 't', 'u', 'r', 'n')) {
                token = tokens[C_TOK_NORETURN];
                goto lbl_push_token;
              }
              break;
            case 'S':
              if (i == 14 && __M6(2, 't', 'a', 't', 'i', 'c', '_') && __M6(8, 'a', 's', 's', 'e', 'r', 't')) {
                token = tokens[C_TOK_STATIC_ASSERT];
                goto lbl_push_token;
              }
              break;
            case 'T':
              if (i == 13 && __M6(2, 'h', 'r', 'e', 'a', 'd', '_') && __M5(8, 'l', 'o', 'c', 'a', 'l')) {
                token = tokens[C_TOK_THREAD_LOCAL];
                goto lbl_push_token;
              }
              break;
            case '_':
              if (M6(2, 'f', 'u', 'n', 'c', '_', '_')) {
                token = tokens[C_TOK_FUNC_NAME];
                goto lbl_push_token;
              }
            default:
              break;
          }
        default:
        lbl_push_ident:
          token = tokens[C_TOK_IDENTIFIER];
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
      token = tokens[C_TOK_NUMBER];
      set_loc;
      set_s;
      push_token;
    } else {
      switch (peek) {
        case '!':
          if (peekn(1) == '=') {
            push(C_TOK_NEQ);
            break;
          }
          push('!');
          break;
        case '#':
          if (peekn(1) == '#') {
            push(C_TOK_TOKEN_PASTE);
            break;
          }
          push('#');
          break;
        case '%':
          if (peekn(1) == '=') {
            push(C_TOK_MOD_ASSIGN);
            break;
          }
          push('%');
          break;
        case '&':
          if (peekn(1) == '&') {
            push(C_TOK_LOGICAL_AND);
            break;
          }
          if (peekn(1) == '=') {
            push(C_TOK_AND_ASSIGN);
            break;
          }
          push('&');
          break;
        case '(':
          push('(');
          break;
        case ')':
          push(')');
          break;
        case '*':
          if (peekn(1) == '=') {
            push(C_TOK_MUL_ASSIGN);
            break;
          }
          push('*');
          break;
        case '+':
          if (peekn(1) == '=') {
            push(C_TOK_PLUS_ASSIGN);
            break;
          }
          if (peekn(1) == '+') {
            push(C_TOK_INCREMENT);
            break;
          }
          push('+');
          break;
        case ',':
          push(',');
          break;
        case '-':
          if (peekn(1) == '=') {
            push(C_TOK_MINUS_ASSIGN);
            break;
          }
          if (peekn(1) == '-') {
            push(C_TOK_DECREMENT);
            break;
          }
          if (peekn(1) == '>') {
            push(C_TOK_ARROW);
            break;
          }
          push('-');
          break;
        case '.':
          if (peekn(1) == '.' && peekn(2) == '.') {
            push(C_TOK_DOTS);
            break;
          }
          push('.');
          break;
        case '/':
          if (peekn(1) == '*') {
            while (next != 0) {
              if (peek == '*') {
                while (next == '*');
                if (peek == '/') {
                  next;
                  break;
                }
              } else if (peekn(1) == 0) {
                exit(1);
              } else {
                switch (peek) {
                  case '\r':
                    if (peekn(1) == '\n') {
                      next;
                    }
                    push('\n');
                    ++self->loc.lineno;
                    self->loc.colno = 0;
                    break;
                  case '\v':
                  case '\f':
                  case '\n':
                    push('\n');
                    ++self->loc.lineno;
                    self->loc.colno = 0;
                    break;
                  default:
                    break;
                }
              }
            }
            break;
          }
          if (peekn(1) == '=') {
            push(C_TOK_DIV_ASSIGN);
            break;
          }
          push('/');
          break;
        case ':':
          push(':');
          break;
        case ';':
          push(';');
          break;
        case '<':
          if (peekn(1) == '<') {
            if (peekn(2) == '=') {
              push(C_TOK_LSHIFT_ASSIGN);
              break;
            }
            push(C_TOK_LSHIFT);
            break;
          }
          if (peekn(1) == '=') {
            push(C_TOK_LEQ);
            break;
          }
          push('<');
          break;
        case '=':
          if (peekn(1) == '=') {
            push(C_TOK_EQ);
            break;
          }
          push('=');
          break;
        case '>':
          if (peekn(1) == '>') {
            if (peekn(2) == '=') {
              push(C_TOK_RSHIFT_ASSIGN);
              break;
            }
            push(C_TOK_RSHIFT);
            break;
          }
          if (peekn(1) == '=') {
            push(C_TOK_GEQ);
            break;
          }
          push('>');
          break;
        case '?':
          push('?');
          break;
        case '[':
          push('[');
          break;
        case '\\':
          push('\\');
          break;
        case ']':
          push(']');
          break;
        case '^':
          if (peekn(1) == '=') {
            push(C_TOK_XOR_ASSIGN);
            break;
          }
          push('^');
          break;
        case '{':
          push('{');
          break;
        case '|':
          if (peekn(1) == '|') {
            push(C_TOK_LOGICAL_OR);
            break;
          }
          if (peekn(1) == '=') {
            push(C_TOK_OR_ASSIGN);
            break;
          }
          push('|');
          break;
        case '}':
          push('}');
          break;
        case '~':
          push('~');
          break;
        case '\r':
          if (peekn(1) == '\n') {
            next;
          }
          push('\n');
          ++self->loc.lineno;
          self->loc.colno = 0;
          break;
        case '\v':
        case '\f':
        case '\n':
          push('\n');
          ++self->loc.lineno;
          self->loc.colno = 0;
          break;
        case ' ':
        case '\t':
        default:
          next;
          break;
      }
    }
  }
  if (n) {
    token = tokens[C_TOK_END];
    set_loc;
    push_token;
  }
}
