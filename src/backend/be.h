#ifndef _JAYL_BE_H_
#define _JAYL_BE_H_

#include <fir_ctx.h>

typedef struct _be_t {
  void (*init)(struct _be_t *this, fir_ctx_t *ctx);
  void (*flush)(struct _be_t *this, fir_ctx_t *ctx);
} be_t;

void be_ctor(be_t *this);

#endif /* _JAYL_BE_H_ */
