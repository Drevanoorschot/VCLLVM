#ifndef VCLLVM_FUNCTIONDECLARER_H
#define VCLLVM_FUNCTIONDECLARER_H

#include <llvm/IR/PassManager.h>
#include "AST/AST.h"

namespace llvm {
    class FunctionDeclarer : public PassInfoMixin<FunctionDeclarer> {
    private:
        std::shared_ptr<AST::Program> p_AST;
    public:
        explicit FunctionDeclarer(std::shared_ptr<AST::Program> p_AST);
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    };

}
#endif //VCLLVM_FUNCTIONDECLARER_H
