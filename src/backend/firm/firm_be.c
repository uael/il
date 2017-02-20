#include "firm_be.h"

#include <libfirm/firm.h>

void firm_init(be_t *this, fir_ctx_t *ctx) {
  ir_init();
}

void firm_flush(be_t *this, fir_ctx_t *ctx) {
  ir_finish();
}