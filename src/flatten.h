#ifndef _JAYL_FLATTEN_H
#define _JAYL_FLATTEN_H

#include "ir.h"

namespace jayl {
  namespace ir {

    Stmt flattenIndexExpressions(Stmt stmt);

    Func flattenIndexExpressions(Func func);

  }
} // namespace jayl::ir

#endif
