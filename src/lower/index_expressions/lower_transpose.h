#ifndef _JAYL_LOWER_TRANPOSE_H
#define _JAYL_LOWER_TRANPOSE_H

#include "ir.h"

namespace jayl {
  namespace ir {

    Stmt lowerTranspose(Var target, const IndexExpr *indexExpression,
      Environment *env, Storage *storage);

  }
}
#endif
