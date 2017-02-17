#ifndef _JAYL_FIR_SYMBOL_H_
#define _JAYL_FIR_SYMBOL_H_

#include "fir_scope.h"

TYPEDEF_STRUCTS(
  fir_symbol_t
);

struct _fir_symbol_t {
  const char *string;
  unsigned id;
  fir_ent_t *entity;
  const fir_scope_t *scope;
};

#endif /* _JAYL_FIR_SYMBOL_H_ */
