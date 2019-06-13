//===- LoopCustomOptz.cpp - Experiment with order of Loop Transformations ---------------------===//
// Experimental pass added by: Aniket Shivam
//===----------------------------------------------------------------------===//

#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO.h"
#include <algorithm>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "loop-custom-optz"

namespace {

class LoopCustomOptz : public ModulePass {
public:
  static char ID;

  LoopCustomOptz()
    : ModulePass(ID) {
    initializeLoopCustomOptzPass(*PassRegistry::getPassRegistry());
  }

  bool runOnModule(Module &M) override;

private:

};

} // end anonymous namespace

char LoopCustomOptz::ID = 0;

INITIALIZE_PASS(LoopCustomOptz, "loop-custom-optz", 
                "Custom ordering of Loop Transformations", false, false)

ModulePass *llvm::createLoopCustomOptzPass() {
  return new LoopCustomOptz();
}

bool LoopCustomOptz::runOnModule(Module &M) {
  if (skipModule(M))
    return false;

  bool Changed = false;

  LLVM_DEBUG(dbgs() << "Enabling custom loop transformation order.\n");

  return Changed;
}


