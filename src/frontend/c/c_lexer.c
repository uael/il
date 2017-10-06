/*
 * libil - Intermediate Language cross-platform c library
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

#include <stdlib.h>
#include <ctype.h>

#include "il/adt/xmalloc.h"
#include "il/adt/string.h"

#include "c_lexer.h"

#include "c_pp.h"
#include "il/compiler.h"

static void c_lexer_enqueue(il_lexer_t *self, unsigned n);

void c_lexer_init(il_lexer_t *self) {
  il_lexer_event_t event;

  self->enqueue = c_lexer_enqueue;
  event = (il_lexer_event_t) {
    .kind = IL_LEXER_EVENT_ON_PUSH,
    .callback = c_pp_on_push_callback,
    .dtor = c_pp_on_push_dtor,
    .data = xmalloc(sizeof(c_pp_t))
  };
  c_pp_init(event.data);
  il_lexer_attach(self, event);
}

#define EMPTY {0}
#define SYNTX(t, s) {.type=(t), .loc={0}, .name=s, .value=s, .length=sizeof(s)-1, .kind=IL_TOKEN_SYNTAX}
#define XSYNT(t, s, n) {.type=(t), .loc={0}, .name=n, .value=s, .length=sizeof(s)-1, .kind=IL_TOKEN_SYNTAX}
#define KEYWD(t, s) {.type=(t), .loc={0}, .name=s, .length=sizeof(s)-1, .kind=IL_TOKEN_KEYWORD}
#define TOKEN(t, k, n) {.type=(t), .loc={0}, .name=n, .kind=(k)}

#define peek *(self->buffer + self->loc.position)
#define peekn(n) (self->buffer + self->loc.position)[n]
#define next (self->loc.colno++, self->loc.position++, peek)
#define nextn(n) (self->loc.colno+=n, self->loc.position+=n, peek)

#define set_loc token.loc = self->loc; token.leading_ws = lws; lws = 0
#define set_s do { \
    token.length = i; \
    token.loc.colno -= i; \
    token.loc.position -= i; \
    token.value = il_strndup(self->compiler, s, i); \
  } while (false)
#define push_token do { \
    if (il_lexer_push(self, token)) n--; \
  } while (false)
#define push(t) do { \
    token = tokens[t]; \
    set_loc; \
    (void) nextn(token.length); \
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


static void c_lexer_enqueue(il_lexer_t *self, unsigned n) {
  static __const il_token_t tokens[] = {
    /* 0x00 */
    XSYNT(C_TOK_END, "\0", "END"),
    KEYWD(C_TOK_AUTO, "auto"),
    KEYWD(C_TOK_BREAK, "break"),
    KEYWD(C_TOK_CASE, "case"),
    KEYWD(C_TOK_CHAR, "char"),
    KEYWD(C_TOK_CONST, "__const"),
    KEYWD(C_TOK_CONTINUE, "continue"),
    KEYWD(C_TOK_DEFAULT, "default"),

    /* 0x08 */
    KEYWD(C_TOK_DO, "do"),
    KEYWD(C_TOK_DOUBLE, "double"),
    XSYNT(C_TOK_EOL, "\n", "EOL"),
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
    KEYWD(C_TOK_ALIGNAS, "_Alignas"),
    KEYWD(C_TOK_ALIGNOF, "_Alignof"),
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
    SYNTX(C_TOK_LE_OP, "<="),
    SYNTX(C_TOK_GE_OP, ">="),
    SYNTX(C_TOK_EQ_OP, "=="),
    SYNTX(C_TOK_NE_OP, "!="),
    SYNTX(C_TOK_PTR_OP, "->"),

    /* 0x48 */
    SYNTX(C_TOK_INCREMENT, "++"),
    SYNTX(C_TOK_DECREMENT, "--"),
    SYNTX(C_TOK_LEFT_OP, "<<"),
    SYNTX(C_TOK_RIGHT_OP, ">>"),
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
    TOKEN(C_TOK_NUMBER, IL_TOKEN_NUMBER, "Number"),
    TOKEN(C_TOK_IDENTIFIER, IL_TOKEN_IDENTIFIER, "Identifier"),
    TOKEN(C_TOK_STRING, IL_TOKEN_STRING_LITERAL, "String"),
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
  il_token_t token;
  unsigned i;
  uint32_t lws = 0;

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
          token.loc.colno -= i;
          token.loc.position -= i;
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
            (void) next;
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
            push(C_TOK_NE_OP);
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
            push(C_TOK_PTR_OP);
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
                  (void) next;
                  break;
                }
              } else if (peekn(1) == 0) {
                /* todo */
                exit(1);
              } else {
                switch (peek) {
                  case '\r':
                    if (peekn(1) == '\n') {
                      (void) next;
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
            push(C_TOK_LEFT_OP);
            break;
          }
          if (peekn(1) == '=') {
            push(C_TOK_LE_OP);
            break;
          }
          push('<');
          break;
        case '=':
          if (peekn(1) == '=') {
            push(C_TOK_EQ_OP);
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
            push(C_TOK_RIGHT_OP);
            break;
          }
          if (peekn(1) == '=') {
            push(C_TOK_GE_OP);
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
            (void) next;
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
          ++lws;
          (void) next;
          break;
        case '\t':
        default:
          (void) next;
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
