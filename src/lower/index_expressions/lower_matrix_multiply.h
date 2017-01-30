#ifndef _JAYL_LOWER_MATRIX_MULTIPLY_H
#define _JAYL_LOWER_MATRIX_MULTIPLY_H

#include "ir.h"

namespace jayl {
  namespace ir {

    Stmt lowerMatrixMultiply(Var target, const IndexExpr *indexExpression,
      Environment *env, Storage *storage);

  }
}
#endif
