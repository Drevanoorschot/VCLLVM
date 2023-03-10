#ifndef VCLLVM_FUNCTIONDECLARER_H
#define VCLLVM_FUNCTIONDECLARER_H

#include <llvm/IR/PassManager.h>
#include "col.pb.h"

/**
 * Pass that creates a signature for a LLVMFunctionDefinition in COL and exposes an <code>FDResult</code> object that
 * binds the the LLVM IR Function to a LLVMFunctionDefinition COL object.
 */
namespace llvm {
    namespace col = vct::col::serialize;

    class FDResult {
    private:
        col::LlvmFunctionDefinition *associatedColFuncDef;
    public:
        explicit FDResult(col::LlvmFunctionDefinition *colFuncDef);

        col::LlvmFunctionDefinition *getAssociatedColFuncDef();
    };

    class FunctionDeclarer : public AnalysisInfoMixin<FunctionDeclarer> {
        friend AnalysisInfoMixin<FunctionDeclarer>;
        static AnalysisKey Key;
    private:
        std::shared_ptr<col::Program> pProgram;
    public:
        using Result = FDResult;

        explicit FunctionDeclarer(std::shared_ptr<col::Program> pProgram);

        Result run(Function &F, FunctionAnalysisManager &FAM);

    };

    class FunctionDeclarerPass : public AnalysisInfoMixin<FunctionDeclarerPass> {
    private:
        std::shared_ptr<col::Program> pProgram;
    public:
        explicit FunctionDeclarerPass(std::shared_ptr<col::Program> pProgram);

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    };
}
#endif //VCLLVM_FUNCTIONDECLARER_H