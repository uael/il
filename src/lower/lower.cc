#include "lower.h"

#include <fstream>

#include "lower_maps.h"
#include "index_expressions/lower_index_expressions.h"

#include "lower_accesses.h"
#include "lower_prints.h"
#include "lower_string_ops.h"
#include "lower_stencil_assemblies.h"

#include "inline.h"
#include "timers.h"
#include "temps.h"
#include "flatten.h"
#include "insert_frees.h"
#include "ir_transforms.h"
#include "ir_printer.h"

#ifdef GPU
#include "backend/gpu/gpu_backend.h"
#endif

using namespace std;

namespace jayl {
  extern std::string kBackend;

  namespace ir {

    static
    Func rewriteCallGraph(const Func &func, const function<Func(Func)> &rewriter) {
      class Rewriter : public jayl::ir::IRRewriterCallGraph {
        public:
        Rewriter(const function<Func(Func)> &rewriter) : rewriter(rewriter) {}

        const function<Func(Func)> &rewriter;

        using IRRewriter::visit;

        void visit(const jayl::ir::Func *op) {
          if (op->getKind() != jayl::ir::Func::Internal) {
            func = *op;
            return;
          }
          func = jayl::ir::Func(*op, rewrite(op->getBody()));
          func = rewriter(func);
        }
      };
      return Rewriter(rewriter).rewrite(func);
    }

    void visitCallGraph(Func func, const function<void(Func)> &visitRule) {
      class Visitor : public jayl::ir::IRVisitorCallGraph {
        public:
        Visitor(const function<void(Func)> &visitRule) : visitRule(visitRule) {}

        const function<void(Func)> &visitRule;

        using jayl::ir::IRVisitor::visit;

        void visit(const jayl::ir::Func *op) {
          if (op->getKind() != jayl::ir::Func::Internal) {
            return;
          }
          jayl::ir::IRVisitorCallGraph::visit(op);
          visitRule(*op);
        }
      };
      Visitor visitor(visitRule);
      func.accept(&visitor);
    }

    static inline
    void printTimedCallGraph(string headerText, Func func, ostream *os) {
      stringstream ss;
      jayl::ir::IRPrinterCallGraph(ss).print(func);
      TimerStorage::getInstance().addSourceLines(ss);
      if (os) {
        *os << ss.rdbuf();
      }
    }

    static inline
    void printCallGraph(string headerText, Func func, ostream *os) {
      if (os) {
        *os << "%% " << headerText << endl;
        jayl::ir::IRPrinterCallGraph(*os).print(func);
        *os << endl;
      }
    }

    Func lower(Func func, std::ostream *os, bool time) {
#ifdef GPU
      // Rewrite system assignments
      if (kBackend == "gpu") {
        func = rewriteCallGraph(func, rewriteSystemAssigns);
        printCallGraph("Rewrite System Assigns (GPU)", func, os);
      }
#endif

      // Inline function calls
      func = rewriteCallGraph(func, inlineCalls);
      printCallGraph("Inline Fuction Calls", func, os);

      // Flatten index expressions and insert temporaries
      func = rewriteCallGraph(func, (Func(*)(Func)) flattenIndexExpressions);
      func = rewriteCallGraph(func, insertTemporaries);
      printCallGraph("Insert Temporaries and Flatten Index Expressions", func, os);

      // Determine Storage
      func = rewriteCallGraph(func, [](Func func) -> Func {
        updateStorage(func, &func.getStorage(), &func.getEnvironment());
        return func;
      });
      if (os) {
        *os << "%% Tensor storage" << endl;
        visitCallGraph(func, [os](Func func) {
          *os << "func " << func.getName() << ":" << endl;
          for (auto &var : func.getStorage()) {
            *os << "  " << var << " : " << func.getStorage().getStorage(var) << endl;
          }
          *os << endl;
        });
        *os << endl;
      }

      func = rewriteCallGraph(func, insertFrees);
      printCallGraph("Insert Frees", func, os);

      func = rewriteCallGraph(func, lowerStringOps);
      func = rewriteCallGraph(func, lowerPrints);
      printCallGraph("Lower String Operations and Prints", func, os);

      // Lower field accesses
      func = rewriteCallGraph(func, lowerFieldAccesses);
      printCallGraph("Lower Field Accesses", func, os);

      // Lower stencil assemblies
      func = rewriteCallGraph(func, lowerStencilAssemblies);
      printCallGraph("Normalize Row Indices", func, os);

      // Lower maps
      func = rewriteCallGraph(func, lowerMaps);
      printCallGraph("Lower Maps", func, os);

#ifdef GPU
      // GPU backend wants memsets as loops over set domains
      if (kBackend == "gpu") {
        func = rewriteCallGraph(func, rewriteMemsets);
        printCallGraph("Rewrite Memsets (GPU)", func, os);
      }
#endif

      // Lower Index Expressions
      func = rewriteCallGraph(func, lowerIndexExpressions);
      printCallGraph("Lower Index Expressions", func, os);

      // Lower Tensor Reads and Writes
      func = rewriteCallGraph(func, lowerTensorAccesses);
      printCallGraph("Lower Tensor Reads and Writes", func, os);

      // Insert timers
      if (time) {
        printTimedCallGraph("Insert Timers", func, os);
        func = rewriteCallGraph(func, insertTimers);
        printCallGraph("Insert Timers", func, os);
      }

      // Lower to GPU Kernels
#if GPU
      if (kBackend == "gpu") {
        func = rewriteCallGraph(func, rewriteCompoundOps);
        printCallGraph("Rewrite Compound Ops (GPU)", func, os);
        func = rewriteCallGraph(func, shardLoops);
        printCallGraph("Shard Loops", func, os);
        func = rewriteCallGraph(func, rewriteVarDecls);
        printCallGraph("Rewritten Var Decls", func, os);
        func = rewriteCallGraph(func, localizeTemps);
        printCallGraph("Localize Temps", func, os);
        func = rewriteCallGraph(func, kernelRWAnalysis);
        printCallGraph("Kernel RW Analysis", func, os);
        func = rewriteCallGraph(func, fuseKernels);
        printCallGraph("Fuse Kernels", func, os);
      }
#endif
      return func;
    }

  }
}
