#ifndef VCLLVM_VARNAMER_H
#define VCLLVM_VARNAMER_H
#include <llvm/IR/PassManager.h>
#include "AST/AST.h"
namespace llvm {

    class VariableNamer : public PassInfoMixin<VariableNamer> {
    private:
        std::shared_ptr<AST::Program> pAst;
    public:
        explicit VariableNamer(std::shared_ptr<AST::Program> pAst);
        PreservedAnalyses run(Function &f, FunctionAnalysisManager &am);
    };

}
#endif //VCLLVM_VARNAMER_H
