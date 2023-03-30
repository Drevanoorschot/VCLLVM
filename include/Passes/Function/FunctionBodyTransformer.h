#ifndef VCLLVM_FUNCTIONBODYTRANSFORMER_H
#define VCLLVM_FUNCTIONBODYTRANSFORMER_H

#include <llvm/IR/PassManager.h>
#include "col.pb.h"

namespace vcllvm {
    using namespace llvm;
    namespace col = vct::col::serialize;

    class FunctionCursor {
    private:
        col::Scope &functionScope;

        std::unordered_map<llvm::Value *, col::Variable *> variableMap;

        std::unordered_map<llvm::BasicBlock *, col::Block *> llvmBlock2ColBlock;
    public:
        explicit FunctionCursor(col::Scope &functionScope);

        col::Scope &getFunctionScope();

        void addVariableMapEntry(llvm::Value &llvmValue, col::Variable &colVar);

        col::Variable &getVariableMapEntry(llvm::Value &llvmValue);

        col::Block &setAndMapLlvmBlock2ColBlock(llvm::BasicBlock &llvmBlock, col::Block &parentColBlock);

        col::Block &getLlvmBlock2ColBlockEntry(llvm::BasicBlock &llvmBlock);

        bool isVisited(llvm::BasicBlock &llvmBlock);

    };

    class FunctionBodyTransformerPass : public PassInfoMixin<FunctionBodyTransformerPass> {
    private:
        std::shared_ptr<col::Program> pProgram;

    public:
        explicit FunctionBodyTransformerPass(std::shared_ptr<col::Program> pProgram);

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    };
}
#endif //VCLLVM_FUNCTIONBODYTRANSFORMER_H
