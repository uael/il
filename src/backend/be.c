#include "be.h"
#include <assert.h>

#ifdef WITH_LIBFIRM
#include "firm/firm_be.h"
#endif

void be_ctor(be_t *this) {
#ifdef WITH_LIBFIRM
  this->init = firm_init;
  this->flush = firm_flush;
#else
  assert(0);
#endif
}
