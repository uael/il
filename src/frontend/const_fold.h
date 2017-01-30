#ifndef _JAYL_CONST_FOLD_H
#define _JAYL_CONST_FOLD_H

#include "fir.h"
#include "fir_rewriter.h"

namespace jayl {
  namespace fir {

// Constant folding pass for taking care of negation and transpose of dense 
// tensor literals. Needed since tests can only have literals as operands.
    class ConstantFolding : public FIRRewriter {
      private:
      virtual void visit(NegExpr::Ptr);

      virtual void visit(TransposeExpr::Ptr);
    };

  }
}

#endif

