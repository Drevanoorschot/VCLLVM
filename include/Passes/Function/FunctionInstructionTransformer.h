#ifndef VCLLVM_FUNCTIONINSTRUCTIONTRANSFORMER_H
#define VCLLVM_FUNCTIONINSTRUCTIONTRANSFORMER_H

#include <llvm/IR/PassManager.h>
#include "col.pb.h"
#include "Passes/Function/BlockMapper.h"


/**
 * Pass that transforms instructions to COL expressions and statements.
 * It starts at the first instruction of the functions entry block and transformers subsequent instructions
 * based on the current state of the function.
 * It has similar structure to the <code>BlockMapper</code>
 */
namespace vcllvm {
    using namespace llvm;
    namespace col = vct::col::serialize;

    /**
     * Helper class that can keep track of state of instructions and has useful helper objects such as the
     * FunctionAnalysisManager, and a look up table for llvm instructions and their corresponding COL expressions.
     */
    class FunctionCursor {
    private:
        BMAResult bmaResult;

        col::Scope *functionScope{};

        std::unordered_map<llvm::Value *, col::Variable *> variableMap;
    public:
        FunctionCursor();

        explicit FunctionCursor(BMAResult bmaResult, col::Scope &functionScope);

        BMAResult &getBmaResult();

        col::Scope &getFunctionScope();


        void addVariableMapEntry(llvm::Value &llvmValue, col::Variable &colVar);


        col::Variable &getVariableMapEntry(llvm::Value &llvmValue);
    };

    class FunctionInstructionTransformerPass : public PassInfoMixin<FunctionInstructionTransformerPass> {
    private:
        std::shared_ptr<col::Program> pProgram;

        FunctionCursor functionCursor;

        void transformBlock(llvm::BasicBlock &llvmBlock);

        void transformRetBlock(llvm::BasicBlock &llvmBlock);

    public:
        explicit FunctionInstructionTransformerPass(std::shared_ptr<col::Program> pProgram);

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    };

    /**
    * converts non terminating instructions (i.e. everything that guarantees to fit into a COL-block)
    */
    void convertNonTermInstruction(llvm::Instruction &llvmInstruction,
                                   col::Block &colBlock,
                                   vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_FUNCTIONINSTRUCTIONTRANSFORMER_H
