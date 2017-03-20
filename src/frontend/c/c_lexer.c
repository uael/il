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
    token.s = jl_strndup(self->fe->compiler, s, i); \
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
    KEYWD(C_TOK_IMAGINARY, "_Imaginary"),
    KEYWD(C_TOK_NORETURN, "_Noreturn"),
    KEYWD(C_TOK_STATIC_ASSERT, "_Static_assert"),

    /* 0x38 */
    KEYWD(C_TOK_THREAD_LOCAL, "_Thread_local"),
    KEYWD(C_TOK_FUNC_NAME, "__func__"),
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
    EMPTY,

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
    {C_TOK_STRING, {0}, NULL, 0, JL_TOKEN_STRING},
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
          if (ML(3, 1, 'u', 't', 'o')) {
            token = tokens[C_TOK_AUTO];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'b':
          if (ML(4, 1, 'r', 'e', 'a', 'k')) {
            token = tokens[C_TOK_BREAK];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'c':
          if (ML(3, 1, 'a', 's', 'e')) {
            token = tokens[C_TOK_CASE];
            goto lbl_push_token;
          }
          if (ML(3, 1, 'h', 'a', 'r')) {
            token = tokens[C_TOK_CHAR];
            goto lbl_push_token;
          }
          if (i >= 4 && s[1] == 'o' && s[2] == 'n') {
            if (ML(2, 3, 's', 't')) {
              token = tokens[C_TOK_CONST];
              goto lbl_push_token;
            }
            if (ML(5, 3, 't', 'i', 'n', 'u', 'e')) {
              token = tokens[C_TOK_CONTINUE];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'd':
          if (ML(6, 1, 'e', 'f', 'a', 'u', 'l', 't')) {
            token = tokens[C_TOK_DEFAULT];
            goto lbl_push_token;
          }
          if (i > 1 && s[1] == 'o') {
            if (i == 2) {
              token = tokens[C_TOK_DO];
              goto lbl_push_token;
            }
            if (ML(4, 2, 'u', 'b', 'l', 'e')) {
              token = tokens[C_TOK_DOUBLE];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'e':
          if (ML(3, 1, 'l', 's', 'e')) {
            token = tokens[C_TOK_ELSE];
            goto lbl_push_token;
          }
          if (ML(3, 1, 'n', 'u', 'm')) {
            token = tokens[C_TOK_ENUM];
            goto lbl_push_token;
          }
          if (ML(5, 1, 'x', 't', 'e', 'r', 'n')) {
            token = tokens[C_TOK_EXTERN];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'f':
          if (ML(4, 1, 'l', 'o', 'a', 't')) {
            token = tokens[C_TOK_FLOAT];
            goto lbl_push_token;
          }
          if (ML(2, 1, 'o', 'r')) {
            token = tokens[C_TOK_FOR];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'g':
          if (ML(3, 1, 'o', 't', 'o')) {
            token = tokens[C_TOK_GOTO];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'i':
          if (ML(1, 1, 'f')) {
            token = tokens[C_TOK_IF];
            goto lbl_push_token;
          }
          if (ML(5, 1, 'n', 'l', 'i', 'n', 'e')) {
            token = tokens[C_TOK_INLINE];
            goto lbl_push_token;
          }
          if (ML(2, 1, 'n', 't')) {
            token = tokens[C_TOK_INT];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'l':
          if (ML(3, 1, 'o', 'n', 'g')) {
            token = tokens[C_TOK_LONG];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'r':
          if (i > 4 && s[1] == 'e') {
            if (ML(6, 2, 'g', 'i', 's', 't', 'e', 'r')) {
              token = tokens[C_TOK_REGISTER];
              goto lbl_push_token;
            }
            if (ML(4, 2, 't', 'u', 'r', 'n')) {
              token = tokens[C_TOK_RETURN];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 's':
          if (ML(4, 1, 'h', 'o', 'r', 't')) {
            token = tokens[C_TOK_SHORT];
            goto lbl_push_token;
          }
          if (i == 6) {
            switch (s[1]) {
              case 'i':
                if (M(4, 2, 'g', 'n', 'e', 'd')) {
                  token = tokens[C_TOK_SIGNED];
                  goto lbl_push_token;
                }
                if (M(4, 2, 'z', 'e', 'o', 'f')) {
                  token = tokens[C_TOK_SIZEOF];
                  goto lbl_push_token;
                }
                break;
              case 't':
                if (M(4, 2, 'a', 't', 'i', 'c')) {
                  token = tokens[C_TOK_STATIC];
                  goto lbl_push_token;
                }
                if (M(4, 2, 'r', 'u', 'c', 't')) {
                  token = tokens[C_TOK_STRUCT];
                  goto lbl_push_token;
                }
                break;
              case 'w':
                if (M(4, 2, 'i', 't', 'c', 'h')) {
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
          if (ML(6, 1, 'y', 'p', 'e', 'd', 'e', 'f')) {
            token = tokens[C_TOK_TYPEDEF];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case 'u':
          if (i > 4 && s[1] == 'n') {
            if (ML(3, 2, 'i', 'o', 'n')) {
              token = tokens[C_TOK_UNION];
              goto lbl_push_token;
            }
            if (ML(6, 2, 's', 'i', 'g', 'n', 'e', 'd')) {
              token = tokens[C_TOK_UNSIGNED];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'v':
          if (i > 3 && s[1] == 'o') {
            if (ML(2, 2, 'i', 'd')) {
              token = tokens[C_TOK_VOID];
              goto lbl_push_token;
            }
            if (ML(6, 2, 'l', 'a', 't', 'i', 'l', 'e')) {
              token = tokens[C_TOK_VOLATILE];
              goto lbl_push_token;
            }
          }
          goto lbl_push_ident;
        case 'w':
          if (ML(4, 1, 'h', 'i', 'l', 'e')) {
            token = tokens[C_TOK_WHILE];
            goto lbl_push_token;
          }
          goto lbl_push_ident;
        case '_':
          if (ML(7, 1, 'A', 'l', 'i', 'g', 'n', 'o', 'f')) {
            token = tokens[C_TOK_ALIGNOF];
            goto lbl_push_token;
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
