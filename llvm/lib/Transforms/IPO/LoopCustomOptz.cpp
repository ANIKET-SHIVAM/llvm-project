//===- LoopCustomOptz.cpp - Experiment with order of Loop Transformations
//---------------------===//
// Experimental pass added by: Aniket Shivam
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Utils.h"
#include <algorithm>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "loop-custom-optz"

namespace {
struct LoopCustomOptz : public ModulePass {
  static char ID;
  unsigned NumLoops;

  explicit LoopCustomOptz(unsigned numLoops = ~0)
      : ModulePass(ID), NumLoops(numLoops) {
    initializeLoopCustomOptzPass(*PassRegistry::getPassRegistry());
  }

  bool runOnModule(Module &M) override;
  bool runOnFunction(Function &F);

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<DominatorTreeWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addPreserved<LoopInfoWrapperPass>();
    AU.addRequiredID(LoopSimplifyID);
  }

}; // end struct
} // end anonymous namespace

char LoopCustomOptz::ID = 0;

INITIALIZE_PASS_BEGIN(LoopCustomOptz, "loop-custom-optz",
                      "Custom ordering of Loop Transformations", false, false)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(LoopInfoWrapperPass)
INITIALIZE_PASS_DEPENDENCY(LoopSimplify)
INITIALIZE_PASS_END(LoopCustomOptz, "loop-custom-optz",
                    "Custom ordering of Loop Transformations", false, false)

ModulePass *llvm::createLoopCustomOptzPass() { return new LoopCustomOptz(); }

bool LoopCustomOptz::runOnModule(Module &M) {
  LLVM_DEBUG(dbgs() << "Enabling custom loop transformation order.\n");
  LLVM_DEBUG(dbgs() << "---Running on Module.\n");
  if (skipModule(M))
    return false;

  if (M.empty())
    return false;

  if (!NumLoops)
    return false;

  bool Changed = false;

  for (auto I = M.begin(); I != M.end(); I++) {
    Function &F = *I;
    Changed |= runOnFunction(F);
    if (!NumLoops)
      break;
  }
  return Changed;
}

bool LoopCustomOptz::runOnFunction(Function &F) {
  LLVM_DEBUG(dbgs() << "---Running on Function: " << F.getName() << '\n');
  // Do not modify `optnone` functions.
  if (F.hasOptNone())
    return false;

  if (F.empty())
    return false;

  LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();

  // If there are no loops in the function.
  if (LI.empty())
    return false;

  // If there are no loops
  if (LI.begin() == LI.end()) {
    LLVM_DEBUG(dbgs() << "------No loops in function.\n");
    return false;
  }

  for (Loop::iterator loopiter = LI.begin(); loopiter != LI.end(); loopiter++) {
    LLVM_DEBUG(dbgs() << "------Loop name: " << (*loopiter)->getName() << "\n");
  }

  return false;
}
