#ifndef _JAYL_INSERT_FREES_H
#define _JAYL_INSERT_FREES_H

#include "func.h"

namespace jayl {
  namespace ir {

/// Insert a free wherever a sparse tensor allocated by an extern function
/// leaves scope.
    Func insertFrees(Func func);

  }
}
#endif
