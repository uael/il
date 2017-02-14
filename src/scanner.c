#include <p99.h>
#include <jayl/ir.h>
#include "scanner.h"
#include "tokenize.h"

static char *buffer;
static size_t size;

#define peek() *ptr
#define peekn(n) ptr[n]
#define next() ({ loc.col++; *++ptr; })
#define nextn(n) ({ loc.col += n; ptr += n; *ptr; })
#define push(id) do { \
    tok = tokens[id]; \
    tok.loc = loc; \
    deque_push_back(&result, tok); \
  } while (0)

#define M(n, i, c) ident[i + n] == (c)
#define M1(n, a) M(n, 0, a)
#define M2(n, a, b) M1(n, a) && M(n, 1, b)
#define M3(n, a, b, c) M2(n, a, b) && M(n, 2, c)
#define M4(n, a, b, c, d) M3(n, a, b, c) && M(n, 3, d)
#define M5(n, a, b, c, d, e) M4(n, a, b, c, d) && M(n, 4, e)
#define M6(n, a, b, c, d, e, f) M5(n, a, b, c, d, e) && M(n, 5, f)

#define MATCH_(n, op, ...) \
  (P99_NARG(__VA_ARGS__) + n op i && (P99_PASTE2(M, P99_NARG(__VA_ARGS__)(n, __VA_ARGS__))))

#define MATCH(n, ...) \
  MATCH_(n, <=, __VA_ARGS__)

#define TRY_MATCH(id, n, ...) \
  if (MATCH_(n, ==, __VA_ARGS__)) { \
    tok = tokens[id]; \
    goto token; \
  }

/**
 *
 * @param buffer
 * @param filename
 * @return
 */
ir_ts_t lex(const char *buffer, const char *filename) {
  const char *ptr;
  char ident[255];
  ir_ts_t result = {0};
  ir_token_t tok;
  ir_loc_t loc = {filename, 0, 0};
  unsigned i;
  char c;

  if (!buffer && filename) {
    return lex_file(filename);
  }

  assert(buffer);
  ptr = buffer;

  while (peek() != IR_TOK_END) {
    if (peek() == '_' || isalpha(peek())) {
      ident[i = 0] = peek();
      while (next() == '_' || isalnum(peek())) {
        ident[++i] = peek();
      }
      ident[++i] = '\0';

      switch (ident[0]) {
        case 'a':
          TRY_MATCH(IR_TOK_AUTO, 1, 'u', 't', 'o');
        case 'b':
          TRY_MATCH(IR_TOK_BREAK, 1, 'r', 'e', 'a', 'k');
        case 'c':
          TRY_MATCH(IR_TOK_CASE, 1, 'a', 's', 'e');
          TRY_MATCH(IR_TOK_CHAR, 1, 'h', 'a', 'r');
          if (MATCH(1, 'o', 'n')) {
            TRY_MATCH(IR_TOK_CONST, 3, 's', 't');
            TRY_MATCH(IR_TOK_CONTINUE, 3, 't', 'i', 'n', 'u', 'e');
          }
        case 'd':
          TRY_MATCH(IR_TOK_DEFAULT, 1, 'e', 'f', 'a', 'u', 'l', 't');
          if (MATCH(1, 'o')) {
            TRY_MATCH(IR_TOK_DOUBLE, 2, 'u', 'b', 'l', 'e');
            TRY_MATCH(IR_TOK_DO, 2, 'o');
          }
        case 'e':
          TRY_MATCH(IR_TOK_ELSE, 1, 'l', 's', 'e');
          TRY_MATCH(IR_TOK_ENUM, 1, 'n', 'u', 'm');
          TRY_MATCH(IR_TOK_EXTERN, 1, 'x', 't', 'e', 'r', 'n');
        case 'f':
          TRY_MATCH(IR_TOK_FLOAT, 1, 'l', 'o', 'a', 't');
          TRY_MATCH(IR_TOK_FOR, 1, 'o', 'r');
        case 'g':
          TRY_MATCH(IR_TOK_GOTO, 1, 'o', 't', 'o');
        case 'i':
          TRY_MATCH(IR_TOK_IF, 1, 'f');
          if (MATCH(1, 'n')) {
            TRY_MATCH(IR_TOK_INT, 2, 't');
            TRY_MATCH(IR_TOK_INLINE, 2, 'l', 'i', 'n', 'e');
            TRY_MATCH(IR_TOK_INCLUDE, 2, 'c', 'l', 'u', 'd', 'e');
          }
        case 'l':
          TRY_MATCH(IR_TOK_LONG, 1, 'o', 'n', 'g');
        case 'r':
          if (MATCH(1, 'e')) {
            TRY_MATCH(IR_TOK_REGISTER, 2, 'g', 'i', 's', 't', 'e', 'r');
            TRY_MATCH(IR_TOK_RETURN, 2, 't', 'u', 'r', 'n');
          }
        case 's':
          TRY_MATCH(IR_TOK_SHORT, 1, 'h', 'o', 'r', 't');
          TRY_MATCH(IR_TOK_SWITCH, 1, 'w', 'i', 't', 'c', 'h');
          if (MATCH(1, 'i')) {
            TRY_MATCH(IR_TOK_SIGNED, 2, 'g', 'n', 'e', 'd');
            TRY_MATCH(IR_TOK_SIZEOF, 2, 'z', 'e', 'o', 'f');
          } else if (MATCH(1, 't')) {
            TRY_MATCH(IR_TOK_STATIC, 2, 'a', 't', 'i', 'c');
            TRY_MATCH(IR_TOK_STRUCT, 2, 'r', 'u', 'c', 't');
          }
        case 't':
          TRY_MATCH(IR_TOK_TYPEDEF, 1, 'y', 'p', 'e', 'd', 'e', 'f');
        case 'u':
          if (MATCH(1, 'n')) {
            TRY_MATCH(IR_TOK_UNION, 2, 'i', 'o', 'n');
            TRY_MATCH(IR_TOK_UNSIGNED, 2, 's', 'i', 'g', 'n', 'e', 'd');
          } else {
            TRY_MATCH(IR_TOK_USE, 1, 's', 'e');
          }
        case 'v':
          if (MATCH(1, 'o')) {
            TRY_MATCH(IR_TOK_VOID, 2, 'i', 'd');
            TRY_MATCH(IR_TOK_VOLATILE, 2, 'l', 'a', 't', 'i', 'l', 'e');
          }
        case 'w':
          TRY_MATCH(IR_TOK_WHILE, 1, 'h', 'i', 'l', 'e');
        default:
          tok = tokens[IR_TOK_IDENTIFIER];
          tok.loc = loc;
          tok.loc.col -= i;
          tok.d.string = str_init(ident);
          deque_push_back(&result, tok);
          break;
        token:
          tok.loc = loc;
          tok.loc.col -= i;
          deque_push_back(&result, tok);
          break;
      }
    } else if (isdigit(peek()) || (peek() == '.' && isdigit(peekn(1)))) {
      ident[i = 0] = peek();
      while (1) {
        if (isdigit(peek()) || peek() == '.' || peek() == '_') {
          ident[++i] = peek();
          next();
        } else if (isalpha(peek())) {
          if ((tolower(peek()) == 'e' || (tolower(peek()) == 'p'))
            && (peekn(1) == '+' || peekn(1) == '-')) {
            ident[++i] = peek();
            next();
          }
          ident[++i] = peek();
          next();
        } else {
          break;
        }
      }
    } else {
      switch (peek()) {
        case IR_TOK_NOT:
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_NEQ);
            nextn(2);
            break;
          }
          push(IR_TOK_NOT);
          next();
          break;
        case IR_TOK_HASH:
          if (peekn(1) == IR_TOK_HASH) {
            push(IR_TOK_TOKEN_PASTE);
            nextn(2);
            break;
          }
          push(IR_TOK_HASH);
          next();
          break;
        case IR_TOK_MODULO:
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_MOD_ASSIGN);
            nextn(2);
            break;
          }
          push(IR_TOK_MODULO);
          next();
          break;
        case IR_TOK_AND:
          if (peekn(1) == IR_TOK_AND) {
            push(IR_TOK_LOGICAL_AND);
            nextn(2);
            break;
          }
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_AND_ASSIGN);
            nextn(2);
            break;
          }
          push(IR_TOK_AND);
          next();
          break;
        case IR_TOK_OPEN_PAREN:
          push(IR_TOK_OPEN_PAREN);
          next();
          break;
        case IR_TOK_CLOSE_PAREN:
          push(IR_TOK_CLOSE_PAREN);
          next();
          break;
        case IR_TOK_STAR:
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_MUL_ASSIGN);
            nextn(2);
            break;
          }
          push(IR_TOK_STAR);
          next();
          break;
        case IR_TOK_PLUS:
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_PLUS_ASSIGN);
            nextn(2);
            break;
          }
          if (peekn(1) == IR_TOK_PLUS) {
            push(IR_TOK_INCREMENT);
            nextn(2);
            break;
          }
          push(IR_TOK_PLUS);
          next();
          break;
        case IR_TOK_COMMA:
          push(IR_TOK_COMMA);
          next();
          break;
        case IR_TOK_MINUS:
          if (peekn(1) == IR_TOK_MINUS) {
            push(IR_TOK_MINUS_ASSIGN);
            nextn(2);
            break;
          }
          if (peekn(1) == IR_TOK_MINUS) {
            push(IR_TOK_DECREMENT);
            nextn(2);
            break;
          }
          push(IR_TOK_MINUS);
          next();
          break;
        case IR_TOK_DOT:
          if (peekn(1) == IR_TOK_DOT && peekn(2) == IR_TOK_DOT) {
            push(IR_TOK_DOTS);
            nextn(3);
            break;
          }
          push(IR_TOK_DOT);
          next();
          break;
        case IR_TOK_SLASH:
          if (peekn(1) == IR_TOK_STAR) {
            while (next() != IR_TOK_END) {
              if (peek() == IR_TOK_STAR) {
                while (next() == IR_TOK_STAR);
                if (peek() == IR_TOK_SLASH) {
                  next();
                  break;
                }
              } else if (peekn(1) == IR_TOK_END) {
                exit(1);
              } else {
                switch (peek()) {
                  case '\r':
                    next();
                    if (peek() == IR_TOK_EOL) {
                      next();
                    }
                    push(IR_TOK_EOL);
                    ++loc.line;
                    loc.col = 0;
                    break;
                  case '\v':
                  case '\f':
                  case IR_TOK_EOL:
                    next();
                    push(IR_TOK_EOL);
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
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_DIV_ASSIGN);
            nextn(2);
            break;
          }
          push(IR_TOK_SLASH);
          next();
          break;
        case IR_TOK_COLON:
          if (peekn(1) == IR_TOK_COLON) {
            push(IR_TOK_DCOLON);
            nextn(2);
            break;
          }
          push(IR_TOK_COLON);
          next();
          break;
        case IR_TOK_SEMICOLON:
          push(IR_TOK_SEMICOLON);
          next();
          break;
        case IR_TOK_LT:
          if (peekn(1) == IR_TOK_LT) {
            if (peekn(2) == IR_TOK_ASSIGN) {
              push(IR_TOK_LSHIFT_ASSIGN);
              nextn(3);
              break;
            }
            push(IR_TOK_LSHIFT);
            nextn(2);
            break;
          }
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_LEQ);
            nextn(2);
            break;
          }
          push(IR_TOK_LT);
          next();
          break;
        case IR_TOK_ASSIGN:
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_EQ);
            nextn(2);
            break;
          }
          if (peekn(1) == IR_TOK_GT) {
            push(IR_TOK_DARROW);
            nextn(2);
            break;
          }
          push(IR_TOK_ASSIGN);
          next();
          break;
        case IR_TOK_GT:
          if (peekn(1) == IR_TOK_GT) {
            if (peekn(2) == IR_TOK_ASSIGN) {
              push(IR_TOK_RSHIFT_ASSIGN);
              nextn(3);
              break;
            }
            push(IR_TOK_RSHIFT);
            nextn(2);
            break;
          }
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_GEQ);
            nextn(2);
            break;
          }
          push(IR_TOK_GT);
          next();
          break;
        case IR_TOK_QUESTION:
          if (peekn(1) == IR_TOK_QUESTION) {
            push(IR_TOK_DQUESTION);
            nextn(2);
            break;
          }
          push(IR_TOK_QUESTION);
          next();
          break;
        case IR_TOK_OPEN_BRACKET:
          push(IR_TOK_OPEN_BRACKET);
          next();
          break;
        case IR_TOK_CLOSE_BRACKET:
          push(IR_TOK_CLOSE_BRACKET);
          next();
          break;
        case IR_TOK_XOR:
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_XOR_ASSIGN);
            nextn(2);
            break;
          }
          push(IR_TOK_XOR);
          next();
          break;
        case IR_TOK_OPEN_CURLY:
          push(IR_TOK_OPEN_CURLY);
          next();
          break;
        case IR_TOK_OR:
          if (peekn(1) == IR_TOK_OR) {
            push(IR_TOK_LOGICAL_OR);
            nextn(2);
            break;
          }
          if (peekn(1) == IR_TOK_ASSIGN) {
            push(IR_TOK_OR_ASSIGN);
            nextn(2);
            break;
          }
          push(IR_TOK_OR);
          next();
          break;
        case IR_TOK_CLOSE_CURLY:
          push(IR_TOK_CLOSE_CURLY);
          next();
          break;
        case IR_TOK_NEG:
          push(IR_TOK_NEG);
          next();
          break;
        case '\r':
          if (peek() == IR_TOK_EOL) {
            next();
          }
          push(IR_TOK_EOL);
          next();
          ++loc.line;
          loc.col = 0;
          break;
        case '\v':
        case '\f':
        case IR_TOK_EOL:
          push(IR_TOK_EOL);
          next();
          ++loc.line;
          loc.col = 0;
          break;
        case ' ':
        case '\t':
          next();
          break;
        default:
          break;
      }
    }
  }

  return result;
}

ir_ts_t lex_file(const char *filename) {
  char *ptr;
  FILE *stream;
  size_t tail;
  ir_ts_t result = {0};

  assert(filename);
  stream = fopen(filename, "r");
  assert(stream);

  fseek(stream, 0L, SEEK_END);
  tail = (size_t) ftell(stream);
  rewind(stream);
  if (!tail) {
    return result;
  }

  if (!buffer) {
    buffer = malloc(tail + 1);
    size = tail;
  } else if (tail > size) {
    buffer = realloc(buffer, tail + 1);
    size = tail;
  }

  fread(buffer, size, 1, stream);
  fclose(stream);
  buffer[tail] = IR_TOK_END;
  ptr = strdup(buffer);
  result = lex(ptr, filename);
  free(ptr);

  return result;
}
