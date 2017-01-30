#ifndef _JAYL_LOWER_TENSOR_UTILS_H
#define _JAYL_LOWER_TENSOR_UTILS_H

#include <vector>

#include "ir.h"

using namespace std;

namespace jayl {
  namespace ir {

/// Rewrite a statement looped over a block of size `blockSize`, substituting
/// induction variables appropriately.
    Stmt rewriteToBlocked(Stmt stmt, vector<Var> inductionVars, Expr blockSize);

  }
}

#endif
