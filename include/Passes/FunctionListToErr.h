//
// Created by dre on 11/18/22.
//

#ifndef VCLLVM_FUNCTIONLISTTOERR_H
#define VCLLVM_FUNCTIONLISTTOERR_H

#include <llvm/IR/PassManager.h>

namespace llvm {

    class FunctionListToErr : public PassInfoMixin<FunctionListToErr> {
    public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    };

}
#endif
