#ifndef _JAYL_LOWER_INDEXEXPRS_H
#define _JAYL_LOWER_INDEXEXPRS_H

#include "ir.h"

namespace jayl {
  namespace ir {

    Stmt lowerIndexStatement(Stmt stmt, Environment *environment, Storage storage);

  }
}
#endif
