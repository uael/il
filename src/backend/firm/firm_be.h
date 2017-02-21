#ifndef _JAYL_FIRM_BE_H_
#define _JAYL_FIRM_BE_H_

#include "backend/be.h"

void firm_init(be_t *this, jay_ctx_t *ctx);

void firm_flush(be_t *this, jay_ctx_t *ctx);

#endif /* _JAYL_FIRM_BE_H_ */
