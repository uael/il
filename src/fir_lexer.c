#include "fir_lexer.h"

#include "util/strtab.h"

static char *buffer;
static size_t size;

#define peek() *ptr
#define peekn(n) ptr[n]
#define next() ({ loc.col++; *++ptr; })
#define nextn(n) ({ loc.col += n; ptr += n; *ptr; })
#define push(id) do { \
    tok = tokens[id]; \
    tok.loc = loc; \
    deque_push_back(token_stream, tok); \
  } while (0)
#define push_string(...) do { \
    P99_IF_EMPTY(__VA_ARGS__)()({ \
      tok = tokens[__VA_ARGS__]; \
      tok.d.string = str_register(string, i); \
    }) \
    tok.loc = loc; \
    tok.loc.col -= i; \
    deque_push_back(token_stream, tok); \
  } while (0)

#define M(n, i, c) string[i + n] == (c)
#define M1(n, a) M(n, 0, a)
#define M2(n, a, b) M1(n, a) && M(n, 1, b)
#define M3(n, a, b, c) M2(n, a, b) && M(n, 2, c)
#define M4(n, a, b, c, d) M3(n, a, b, c) && M(n, 3, d)
#define M5(n, a, b, c, d, e) M4(n, a, b, c, d) && M(n, 4, e)
#define M6(n, a, b, c, d, e, f) M5(n, a, b, c, d, e) && M(n, 5, f)
#define M7(n, a, b, c, d, e, f, g) M6(n, a, b, c, d, e, f) && M(n, 6, g)
#define M8(n, a, b, c, d, e, f, g, h) M7(n, a, b, c, d, e, f, g) && M(n, 7, h)

#define MATCH_(n, op, ...) \
  (P99_NARG(__VA_ARGS__) + n op i && (P99_PASTE2(M, P99_NARG(__VA_ARGS__)(n, __VA_ARGS__))))

#define MATCH(n, ...) \
  MATCH_(n, <=, __VA_ARGS__)

#define TRY_MATCH(id, n, ...) \
  if (MATCH_(n, ==, __VA_ARGS__)) { \
    tok = tokens[id]; \
    goto token; \
  }

void cleanup() {
  free(buffer);
}

void lex(fir_lexer_t *this, fir_toks_t *token_stream) {
  char *ptr;
  FILE *stream;
  size_t tail;

  assert(this->filename);
  stream = fopen(this->filename, "r");
  assert(stream);

  fseek(stream, 0L, SEEK_END);
  tail = (size_t) ftell(stream);
  rewind(stream);
  if (!tail) {
    return;
  }

  if (!buffer) {
    buffer = malloc(tail + 1);
    size = tail;
    atexit(cleanup);
  } else if (tail > size) {
    buffer = realloc(buffer, tail + 1);
    size = tail;
  }

  fread(buffer, size, 1, stream);
  fclose(stream);
  buffer[tail] = FIR_TOK_END;
  ptr = strdup(buffer);
  this->lex_str(this, ptr, token_stream);
  free(ptr);
}

void lex_str(fir_lexer_t *this, const char *buffer, fir_toks_t *token_stream) {
  const char *ptr;
  char string[255];
  fir_tok_t tok;
  fir_loc_t loc = {this->filename, 0, 0};
  unsigned i;

  if (!buffer && this->filename) {
    this->lex(this, token_stream);
    return;
  }

  assert(buffer);
  ptr = buffer;

  while (peek() != FIR_TOK_END) {
    if (peek() == '_' || isalpha(peek())) {
      string[i = 0] = peek();
      while (next() == '_' || isalnum(peek())) {
        string[++i] = peek();
      }
      string[++i] = '\0';

      switch (string[0]) {
        case 'a':
          TRY_MATCH(FIR_TOK_AUTO, 1, 'u', 't', 'o');
        case 'b':
          TRY_MATCH(FIR_TOK_BREAK, 1, 'r', 'e', 'a', 'k');
        case 'c':
          TRY_MATCH(FIR_TOK_CASE, 1, 'a', 's', 'e');
          TRY_MATCH(FIR_TOK_CHAR, 1, 'h', 'a', 'r');
          if (MATCH(1, 'o', 'n')) {
            TRY_MATCH(FIR_TOK_CONST, 3, 's', 't');
            TRY_MATCH(FIR_TOK_CONTINUE, 3, 't', 'i', 'n', 'u', 'e');
          }
        case 'd':
          TRY_MATCH(FIR_TOK_DEFAULT, 1, 'e', 'f', 'a', 'u', 'l', 't');
          if (MATCH(1, 'o')) {
            TRY_MATCH(FIR_TOK_DOUBLE, 2, 'u', 'b', 'l', 'e');
            TRY_MATCH(FIR_TOK_DO, 2, 'o');
          }
        case 'e':
          TRY_MATCH(FIR_TOK_ELSE, 1, 'l', 's', 'e');
          TRY_MATCH(FIR_TOK_ENUM, 1, 'n', 'u', 'm');
          TRY_MATCH(FIR_TOK_EXTERN, 1, 'x', 't', 'e', 'r', 'n');
        case 'f':
          TRY_MATCH(FIR_TOK_FLOAT, 1, 'l', 'o', 'a', 't');
          TRY_MATCH(FIR_TOK_FOR, 1, 'o', 'r');
        case 'g':
          TRY_MATCH(FIR_TOK_GOTO, 1, 'o', 't', 'o');
        case 'i':
          TRY_MATCH(FIR_TOK_IF, 1, 'f');
          if (MATCH(1, 'n')) {
            TRY_MATCH(FIR_TOK_INT, 2, 't');
            TRY_MATCH(FIR_TOK_INLINE, 2, 'l', 'i', 'n', 'e');
            TRY_MATCH(FIR_TOK_INCLUDE, 2, 'c', 'l', 'u', 'd', 'e');
          }
        case 'l':
          TRY_MATCH(FIR_TOK_LONG, 1, 'o', 'n', 'g');
        case 'n':
          TRY_MATCH(FIR_TOK_NAMESPACE, 1, 'a', 'm', 'e', 's', 'p', 'a', 'c', 'e');
        case 'r':
          if (MATCH(1, 'e')) {
            TRY_MATCH(FIR_TOK_REGISTER, 2, 'g', 'i', 's', 't', 'e', 'r');
            TRY_MATCH(FIR_TOK_RETURN, 2, 't', 'u', 'r', 'n');
          }
        case 's':
          TRY_MATCH(FIR_TOK_SHORT, 1, 'h', 'o', 'r', 't');
          TRY_MATCH(FIR_TOK_SWITCH, 1, 'w', 'i', 't', 'c', 'h');
          if (MATCH(1, 'i')) {
            TRY_MATCH(FIR_TOK_SIGNED, 2, 'g', 'n', 'e', 'd');
            TRY_MATCH(FIR_TOK_SIZEOF, 2, 'z', 'e', 'o', 'f');
          } else if (MATCH(1, 't')) {
            TRY_MATCH(FIR_TOK_STATIC, 2, 'a', 't', 'i', 'c');
            TRY_MATCH(FIR_TOK_STRUCT, 2, 'r', 'u', 'c', 't');
          }
        case 't':
          TRY_MATCH(FIR_TOK_TYPEDEF, 1, 'y', 'p', 'e', 'd', 'e', 'f');
        case 'u':
          if (MATCH(1, 'n')) {
            TRY_MATCH(FIR_TOK_UNION, 2, 'i', 'o', 'n');
            TRY_MATCH(FIR_TOK_UNSIGNED, 2, 's', 'i', 'g', 'n', 'e', 'd');
          } else {
            TRY_MATCH(FIR_TOK_USE, 1, 's', 'e');
          }
        case 'v':
          if (MATCH(1, 'o')) {
            TRY_MATCH(FIR_TOK_VOID, 2, 'i', 'd');
            TRY_MATCH(FIR_TOK_VOLATILE, 2, 'l', 'a', 't', 'i', 'l', 'e');
          }
        case 'w':
          TRY_MATCH(FIR_TOK_WHILE, 1, 'h', 'i', 'l', 'e');
        default:
          push_string(FIR_TOK_IDENTIFIER);
          break;
        token:
          push_string();
          break;
      }
    } else if (isdigit(peek()) || (peek() == '.' && isdigit(peekn(1)))) {
      string[i = 0] = peek();
      while (1) {
        if (isdigit(next()) || peek() == '.' || peek() == '_') {
          string[++i] = peek();
        } else if (isalpha(peek())) {
          if ((tolower(peek()) == 'e' || (tolower(peek()) == 'p'))
            && (peekn(1) == '+' || peekn(1) == '-')) {
            string[++i] = peek();
            next();
          }
          string[++i] = peek();
        } else {
          string[++i] = '\0';
          break;
        }
      }
      push_string(FIR_TOK_PREP_NUMBER);
    } else {
      switch (peek()) {
        case FIR_TOK_NOT:
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_NEQ);
            nextn(2);
            break;
          }
          push(FIR_TOK_NOT);
          next();
          break;
        case FIR_TOK_HASH:
          if (peekn(1) == FIR_TOK_HASH) {
            push(FIR_TOK_TOKEN_PASTE);
            nextn(2);
            break;
          }
          push(FIR_TOK_HASH);
          next();
          break;
        case FIR_TOK_MODULO:
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_MOD_ASSIGN);
            nextn(2);
            break;
          }
          push(FIR_TOK_MODULO);
          next();
          break;
        case FIR_TOK_AND:
          if (peekn(1) == FIR_TOK_AND) {
            push(FIR_TOK_LOGICAL_AND);
            nextn(2);
            break;
          }
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_AND_ASSIGN);
            nextn(2);
            break;
          }
          push(FIR_TOK_AND);
          next();
          break;
        case FIR_TOK_OPEN_PAREN:
          push(FIR_TOK_OPEN_PAREN);
          next();
          break;
        case FIR_TOK_CLOSE_PAREN:
          push(FIR_TOK_CLOSE_PAREN);
          next();
          break;
        case FIR_TOK_STAR:
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_MUL_ASSIGN);
            nextn(2);
            break;
          }
          push(FIR_TOK_STAR);
          next();
          break;
        case FIR_TOK_PLUS:
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_PLUS_ASSIGN);
            nextn(2);
            break;
          }
          if (peekn(1) == FIR_TOK_PLUS) {
            push(FIR_TOK_INCREMENT);
            nextn(2);
            break;
          }
          push(FIR_TOK_PLUS);
          next();
          break;
        case FIR_TOK_COMMA:
          push(FIR_TOK_COMMA);
          next();
          break;
        case FIR_TOK_MINUS:
          if (peekn(1) == FIR_TOK_MINUS) {
            push(FIR_TOK_MINUS_ASSIGN);
            nextn(2);
            break;
          }
          if (peekn(1) == FIR_TOK_MINUS) {
            push(FIR_TOK_DECREMENT);
            nextn(2);
            break;
          }
          push(FIR_TOK_MINUS);
          next();
          break;
        case FIR_TOK_DOT:
          if (peekn(1) == FIR_TOK_DOT && peekn(2) == FIR_TOK_DOT) {
            push(FIR_TOK_DOTS);
            nextn(3);
            break;
          }
          push(FIR_TOK_DOT);
          next();
          break;
        case FIR_TOK_SLASH:
          if (peekn(1) == FIR_TOK_STAR) {
            while (next() != FIR_TOK_END) {
              if (peek() == FIR_TOK_STAR) {
                while (next() == FIR_TOK_STAR);
                if (peek() == FIR_TOK_SLASH) {
                  next();
                  break;
                }
              } else if (peekn(1) == FIR_TOK_END) {
                exit(1);
              } else {
                switch (peek()) {
                  case '\r':
                    next();
                    if (peek() == FIR_TOK_EOL) {
                      next();
                    }
                    push(FIR_TOK_EOL);
                    ++loc.line;
                    loc.col = 0;
                    break;
                  case '\v':
                  case '\f':
                  case FIR_TOK_EOL:
                    next();
                    push(FIR_TOK_EOL);
                    ++loc.line;
                    loc.col = 0;
                    break;
                  default:
                    break;
                }
              }
            }
            break;
          }
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_DIV_ASSIGN);
            nextn(2);
            break;
          }
          push(FIR_TOK_SLASH);
          next();
          break;
        case FIR_TOK_COLON:
          if (peekn(1) == FIR_TOK_COLON) {
            push(FIR_TOK_DCOLON);
            nextn(2);
            break;
          }
          push(FIR_TOK_COLON);
          next();
          break;
        case FIR_TOK_SEMICOLON:
          push(FIR_TOK_SEMICOLON);
          next();
          break;
        case FIR_TOK_LT:
          if (peekn(1) == FIR_TOK_LT) {
            if (peekn(2) == FIR_TOK_ASSIGN) {
              push(FIR_TOK_LSHIFT_ASSIGN);
              nextn(3);
              break;
            }
            push(FIR_TOK_LSHIFT);
            nextn(2);
            break;
          }
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_LEQ);
            nextn(2);
            break;
          }
          push(FIR_TOK_LT);
          next();
          break;
        case FIR_TOK_ASSIGN:
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_EQ);
            nextn(2);
            break;
          }
          if (peekn(1) == FIR_TOK_GT) {
            push(FIR_TOK_DARROW);
            nextn(2);
            break;
          }
          push(FIR_TOK_ASSIGN);
          next();
          break;
        case FIR_TOK_GT:
          if (peekn(1) == FIR_TOK_GT) {
            if (peekn(2) == FIR_TOK_ASSIGN) {
              push(FIR_TOK_RSHIFT_ASSIGN);
              nextn(3);
              break;
            }
            push(FIR_TOK_RSHIFT);
            nextn(2);
            break;
          }
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_GEQ);
            nextn(2);
            break;
          }
          push(FIR_TOK_GT);
          next();
          break;
        case FIR_TOK_QUESTION:
          if (peekn(1) == FIR_TOK_QUESTION) {
            push(FIR_TOK_DQUESTION);
            nextn(2);
            break;
          }
          push(FIR_TOK_QUESTION);
          next();
          break;
        case FIR_TOK_OPEN_BRACKET:
          push(FIR_TOK_OPEN_BRACKET);
          next();
          break;
        case FIR_TOK_CLOSE_BRACKET:
          push(FIR_TOK_CLOSE_BRACKET);
          next();
          break;
        case FIR_TOK_XOR:
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_XOR_ASSIGN);
            nextn(2);
            break;
          }
          push(FIR_TOK_XOR);
          next();
          break;
        case FIR_TOK_OPEN_CURLY:
          push(FIR_TOK_OPEN_CURLY);
          next();
          break;
        case FIR_TOK_OR:
          if (peekn(1) == FIR_TOK_OR) {
            push(FIR_TOK_LOGICAL_OR);
            nextn(2);
            break;
          }
          if (peekn(1) == FIR_TOK_ASSIGN) {
            push(FIR_TOK_OR_ASSIGN);
            nextn(2);
            break;
          }
          push(FIR_TOK_OR);
          next();
          break;
        case FIR_TOK_CLOSE_CURLY:
          push(FIR_TOK_CLOSE_CURLY);
          next();
          break;
        case FIR_TOK_NEG:
          push(FIR_TOK_NEG);
          next();
          break;
        case '\r':
          if (peek() == FIR_TOK_EOL) {
            next();
          }
          push(FIR_TOK_EOL);
          next();
          ++loc.line;
          loc.col = 0;
          break;
        case '\v':
        case '\f':
        case FIR_TOK_EOL:
          push(FIR_TOK_EOL);
          next();
          ++loc.line;
          loc.col = 0;
          break;
        case ' ':
        case '\t':
          next();
          break;
        default:
          next();
          break;
      }
    }
  }
  push(FIR_TOK_END);
}

void fir_lexer_ctor(fir_lexer_t *this, fir_ctx_t *ctx) {
  fir_lexer_ctor2(this, ctx, ctx->in);
}

void fir_lexer_ctor2(fir_lexer_t *this, fir_ctx_t *ctx, const char *in) {
  this->ctx = ctx;
  this->filename = in;
  this->lex = lex;
  this->lex_str = lex_str;
}

void fir_lexer_dtor(fir_lexer_t *this) {

}
