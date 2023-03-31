#ifndef VCLLVM_FUNCTIONBODYTRANSFORMER_H
#define VCLLVM_FUNCTIONBODYTRANSFORMER_H

#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/PassManager.h>

#include "col.pb.h"

namespace vcllvm {
    using namespace llvm;
    namespace col = vct::col::serialize;

    struct LabeledColBlock {
        col::Label &label;
        col::Block &block;
    };

    class FunctionCursor {
    private:
        col::Scope &functionScope;

        col::Block &functionBody;

        llvm::LoopInfo &loopInfo;

        std::unordered_map<llvm::Value *, col::Variable *> variableMap;

        std::unordered_map<llvm::BasicBlock *, LabeledColBlock> llvmBlock2LabeledColBlock;
    public:
        explicit FunctionCursor(col::Scope &functionScope, col::Block &functionBody, llvm::LoopInfo &loopInfo);

        col::Scope &getFunctionScope();

        void addVariableMapEntry(llvm::Value &llvmValue, col::Variable &colVar);

        col::Variable &getVariableMapEntry(llvm::Value &llvmValue);

        LabeledColBlock &getOrSetLlvmBlock2LabeledColBlockEntry(BasicBlock &llvmBlock);

        bool isVisited(llvm::BasicBlock &llvmBlock);

        LoopInfo &getLoopInfo();

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
