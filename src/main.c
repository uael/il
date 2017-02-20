#include "util/opt.h"
#include "fir_parser.h"

#include <assert.h>

#ifdef WITH_LIBFIRM
#include <libfirm/adt/obstack.h>
#endif

static void help(opt_t *this, fir_ctx_t *ctx) {
  fprintf(
    stdout,
    "Usage: %s [-(S|E|c)] [-v] [-I <path>] [-o <file>] <file>\n",
    ctx->program
  );
  exit(1);
}

static void flag(opt_t *this, fir_ctx_t *ctx) {
  switch (*this->o) {
    case 'v':
      ctx->verbose = true;
      break;
    default:
      assert(0);
      break;
  }
}

static void output(opt_t *this, fir_ctx_t *ctx) {
  ctx->out = this->v;
}

static void std(opt_t *this, fir_ctx_t *ctx) {

}

int main(int argc, char *argv[]) {
  int i;
  fir_ctx_t ctx = P99_INIT;
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
  ctx.parser->parse(ctx.parser, NULL);
  ctx_dtor(&ctx);

  return EXIT_SUCCESS;
}
