#ifndef _JAYL_FIR_SCOPE_H_
#define _JAYL_FIR_SCOPE_H_

#include "fir_entity.h"

TYPEDEF_STRUCTS(
  fir_scope_t
);

struct _fir_scope_t {
  fir_ent_t *entities;
  unsigned depth;
};

#endif /* _JAYL_FIR_SCOPE_H_ */
