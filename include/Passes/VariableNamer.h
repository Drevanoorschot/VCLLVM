#ifndef VCLLVM_VARNAMER_H
#define VCLLVM_VARNAMER_H
#include <llvm/IR/PassManager.h>
#include "AST/AST.h"
namespace llvm {

    class VariableNamer : public PassInfoMixin<VariableNamer> {
    private:
        std::shared_ptr<AST::Program> p_AST;
    public:
        explicit VariableNamer(std::shared_ptr<AST::Program> p_AST);
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    };

}
#endif //VCLLVM_VARNAMER_H
