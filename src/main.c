#include "util/opt.h"
#include "frontend/parser.h"

#include <assert.h>
#include <backend/be.h>

static void help(opt_t *this, jay_ctx_t *ctx) {
  fprintf(
    stderr,
    "Usage: %s [-(S|E|c)] [-v] [-I <path>] [-o <file>] <file>\n",
    ctx->program
  );
  exit(1);
}

static void flag(opt_t *this, jay_ctx_t *ctx) {
  switch (*this->o) {
    case 'v':
      ctx->verbose = true;
      break;
    default:
      assert(0);
      break;
  }
}

static void output(opt_t *this, jay_ctx_t *ctx) {
  ctx->out = this->v;
}

static void std(opt_t *this, jay_ctx_t *ctx) {

}

int main(int argc, char *argv[]) {
  int i;
  jay_ctx_t ctx = P99_INIT;
  parser_t parser = P99_INIT;
  lexer_t lexer = P99_INIT;
  be_t be = P99_INIT;
  opt_t optv[] = {
    {"-v:",     &flag},
    {"-o:",     &output},
    {"-std=",    &std},
    {"--help", &help}
  };

  ctx.program = argv[0];
  i = parse_args(sizeof(optv) / sizeof(optv[0]), optv, argc, argv, &ctx);
  if (i == argc - 1) {
    ctx.in = argv[i];
  } else {
    help(NULL, &ctx);
  }

  ctx_ctor(&ctx, &parser, &be);
  parser_ctor(&parser, &ctx, &lexer, NULL);
  lexer_ctor(&lexer, &ctx, NULL);
  be_ctor(&be);

  be.init(&be, &ctx);

  ctx.parser->parse(ctx.parser, NULL);

  be.flush(&be, &ctx);

  ctx_dtor(&ctx);
  parser_dtor(&parser);
  lexer_dtor(&lexer);

  return EXIT_SUCCESS;
}
