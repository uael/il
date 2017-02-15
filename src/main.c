#include <stdlib.h>
#include <stdio.h>
#include "util/opt.h"
#include "parser.h"

static void help(opt_t *this, jayl_ctx_t *ctx) {
  fprintf(
    stderr,
    "Usage: %s [-(S|E|c)] [-v] [-I <path>] [-o <file>] <file>\n",
    ctx->program
  );
  exit(1);
}

static void flag(opt_t *this, jayl_ctx_t *ctx) {
  switch (*this->o) {
    case 'v':
      ctx->verbose = true;
      break;
    default:
      assert(0);
      break;
  }
}

static void output(opt_t *this, jayl_ctx_t *ctx) {
  ctx->out = this->v;
}

static void std(opt_t *this, jayl_ctx_t *ctx) {

}

int main(int argc, char *argv[]) {
  int i;
  jayl_ctx_t ctx = {};
  ir_ts_t token_stream;
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

  ctx_ctor(&ctx);
  token_stream = ctx.lexer->lex(ctx.lexer);

  for (i = 0; i < deque_len(&token_stream); ++i) {
    printf("token: %s\n", str_raw(deque_get(&token_stream, i).d.string));
  }

  ctx_dtor(&ctx);

  return EXIT_SUCCESS;
}