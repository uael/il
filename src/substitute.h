#ifndef _JAYL_SUBSTITUTE_H
#define _JAYL_SUBSTITUTE_H

#include <map>
#include "ir.h"

namespace jayl {
  namespace ir {

    Expr substitute(std::map<Expr, Expr> substitutions, Expr expr);

    Stmt substitute(std::map<Expr, Expr> substitutions, Stmt stmt);

    Expr substitute(Expr oldExpr, Expr newExpr, Expr expr);

    Stmt substitute(Expr oldExpr, Expr newExpr, Stmt stmt);

    Expr substitute(std::map<IndexVar, IndexVar> substitutions, Expr expr);

    Stmt substitute(std::map<IndexVar, IndexVar> substitutions, Stmt stmt);

  }
}

#endif
