#include "../../include/Passes/FunctionListToErr.h"

using namespace llvm;

PreservedAnalyses FunctionListToErr::run(Function &F, FunctionAnalysisManager &AM) {
    errs() << F.getName() << "\n";

    return PreservedAnalyses::all();
}


