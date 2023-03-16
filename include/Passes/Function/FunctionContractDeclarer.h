#ifndef VCLLVM_FUNCTIONCONTRACTDECLARER_H
#define VCLLVM_FUNCTIONCONTRACTDECLARER_H

#include <llvm/IR/PassManager.h>
#include "col.pb.h"

/**
 * Pass that adds an LLVMFunctionContract to its corresponding LLVMFunctionDefinition in the presence
 * of a contract metadata node. Exposes a <code>FCDResult</code> result object for back referencing instruction aliases
 * into the contract.
 */
namespace llvm {
    namespace col = vct::col::serialize;

    class FDCResult {
    private:
        col::LlvmFunctionContract &associatedColFuncContract;
    public:
        explicit FDCResult(col::LlvmFunctionContract &colFuncContract);

        col::LlvmFunctionContract &getAssociatedColFuncContract();
    };

    class FunctionContractDeclarer : public AnalysisInfoMixin<FunctionContractDeclarer> {
        friend AnalysisInfoMixin<FunctionContractDeclarer>;
        static AnalysisKey Key;
    private:
        std::shared_ptr<col::Program> pProgram;
    public:
        using Result = FDCResult;

        explicit FunctionContractDeclarer(std::shared_ptr<col::Program> pProgram);

        Result run(Function &F, FunctionAnalysisManager &FAM);
    };

    class FunctionContractDeclarerPass : public AnalysisInfoMixin<FunctionContractDeclarerPass> {
    private:
        std::shared_ptr<col::Program> pProgram;
    public:
        explicit FunctionContractDeclarerPass(std::shared_ptr<col::Program> pProgram);

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    };
}
#endif //VCLLVM_FUNCTIONCONTRACTDECLARER_H
