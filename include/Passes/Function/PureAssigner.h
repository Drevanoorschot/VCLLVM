#ifndef VCLLVM_PUREASSIGNER_H
#define VCLLVM_PUREASSIGNER_H

#include <llvm/IR/Type.h>
#include <llvm/IR/PassManager.h>
#include "col.pb.h"

namespace llvm {
    namespace col = vct::col::serialize;

    class PureAssignerPass : public PassInfoMixin<PureAssignerPass> {
    private:
        std::shared_ptr<col::Program> pProgram;
    public:
        explicit PureAssignerPass(std::shared_ptr<col::Program> pProgram);

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    };

    void reportError(Function &F, const std::string& explanation);
}
#endif //VCLLVM_PUREASSIGNER_H
