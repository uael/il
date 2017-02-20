#include "jay_parser.h"

void parseUse(struct _fir_parser_t *this, fir_tus_t *translation_units);

void parseNamespace(struct _fir_parser_t *this, fir_tus_t *translation_units);

#if defined(WIN32) || defined(_WIN32)
#define DS "\\"
#else
#define DS "/"
#endif

#define FIR_TOK_NS_SEP FIR_TOK_DCOLON

#define peek() this->peek(this)
#define peekn(n) this->peekn(this, n)
#define next() this->next(this)
#define consume(t) this->consume(this, t)

#define iseot(t) (/*t == FIR_TOK_EOL || */t == FIR_TOK_SEMICOLON)

void jay_parse_str(struct _fir_parser_t *this, const char *buffer, fir_tus_t *translation_units) {
  if (!translation_units) {
    translation_units = &this->ctx->prg.tus;
  }
  this->lexer->lex_str(this->lexer, buffer, &this->toks);

  return this->parse(this, translation_units);
}

void jay_parse(struct _fir_parser_t *this, fir_tus_t *translation_units) {
  fir_tok_t tok;

  if (!deque_len(&this->toks)) {
    this->lexer->lex(this->lexer, &this->toks);
  }
  if (!translation_units) {
    translation_units = &this->ctx->prg.tus;
  }

  while ((tok = peek()).kind != FIR_TOK_END) {
    switch (tok.kind) {
      case FIR_TOK_INCLUDE:
        perror("Implementation pending.");
        exit(1);
      case FIR_TOK_USE:
        parseUse(this, translation_units);
        break;
      case FIR_TOK_NAMESPACE:
      case FIR_TOK_IDENTIFIER:
      default:
        printf("token: %s\n", str_raw(tok.d.string));
        next();
        break;
    }
  }
}

void parseUse(struct _fir_parser_t *this, fir_tus_t *translation_units) {
  fir_tok_t tok;
  fir_lexer_t lexer = P99_INIT;
  fir_parser_t parser = P99_INIT;
  char path[256];

  if (!(tok = consume(FIR_TOK_USE)).is_id_or_kw) {
    perror("Expected identifier or keyword.");
    exit(1);
  }

  strcpy(path, this->src_dir);
  strcat(path, DS);
  strcat(path, str_raw(tok.d.string));
  next();
  while (1) {
    tok = peek();
    if (iseot(tok.kind)) {
      break;
    }
    if (!(tok = consume(FIR_TOK_NS_SEP)).is_id_or_kw) {
      fprintf(stderr,
        "Syntax Error: Expected identifier or keyword, got '%s'.\n",
        str_raw(tok.d.string)
      );
      exit(EXIT_FAILURE);
    }
    strcat(path, DS);
    strcat(path, str_raw(tok.d.string));
    next();
  }
  consume(FIR_TOK_SEMICOLON);

  strcat(path, JAY_SRC_EXT);

  fir_lexer_ctor(&lexer, this->ctx, path);
  fir_parser_ctor(&parser, this->ctx, &lexer, this->src_dir);

  parser.parse(&parser, translation_units);
}