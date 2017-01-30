#ifndef _JAYL_LOWER_SCATTER_WORKSPACE_H
#define _JAYL_LOWER_SCATTER_WORKSPACE_H

#include <vector>
#include "ir.h"
#include "loops.h"

namespace jayl {
  namespace ir {

/// Lower the index expression to a loop nest that iterates over operand indices
/// and scatters computed values into a dense workspace. In the course of
/// lowering this functon may add arrays and indices to the environment.
    Stmt lowerScatterWorkspace(Var target, const IndexExpr *indexExpression,
      Environment *env, Storage *storage);

  }
}
#endif
