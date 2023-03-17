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
 * TODO This gonna be a big boy class, think about how this class could potentially be split up more effectively.
 */
namespace vcllvm {
    using namespace llvm;
    namespace col = vct::col::serialize;

    /**
     * Helper class that can keep track of state of instructions and has useful helper objects such as the
     * FunctionAnalysisManager, and a look up table for llvm instructions and their corresponding COL expressions.
     */
    // TODO this is some weird ass shit, if segfaults and empty pointers start to appear, start looking here :P
    using LLVMVar = std::variant<llvm::Instruction *, llvm::Argument *>;

    using COLVar = std::variant<col::Variable *>;

    class FunctionCursor {
    private:
        BMAResult bmaResult;

        std::unordered_map<LLVMVar, COLVar> variableMap;
    public:
        FunctionCursor();

        explicit FunctionCursor(BMAResult bmaResult);

        BMAResult &getBMAResult();


        void addVariableMapEntry(LLVMVar llvmVar, COLVar colVar);


        COLVar getVariableMapEntry(LLVMVar llvmVar);
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
                                   llvm2Col::ColScopedBlock colScopedBlock,
                                   vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_FUNCTIONINSTRUCTIONTRANSFORMER_H
