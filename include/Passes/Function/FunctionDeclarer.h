#ifndef VCLLVM_FUNCTIONDECLARER_H
#define VCLLVM_FUNCTIONDECLARER_H

#include <llvm/IR/PassManager.h>
#include "Conversion/Conversion.h"
#include "col.pb.h"

/**
 * Pass that creates a signature for a LLVMFunctionDefinition in COL and exposes an <code>FDResult</code> object that
 * binds the the LLVM IR Function to a LLVMFunctionDefinition COL object.
 */
namespace vcllvm {
    using namespace llvm;
    namespace col = vct::col::serialize;

    class FDResult {
        friend class FunctionDeclarer;
    private:
        col::LlvmFunctionDefinition &associatedColFuncDef;
        llvm2Col::ColScopedBlock associatedScopedColFuncBody;
        std::unordered_map<llvm::Argument *, col::Variable *> funcArgMap;

        void addFuncArgMapEntry(llvm::Argument &llvmArg, col::Variable &colArg);

    public:
        explicit FDResult(col::LlvmFunctionDefinition &colFuncDef, llvm2Col::ColScopedBlock associatedScopedColFuncBody);

        col::LlvmFunctionDefinition &getAssociatedColFuncDef();

        llvm2Col::ColScopedBlock getAssociatedScopedColFuncBody();

        col::Variable &getFuncArgMapEntry(llvm::Argument &arg);
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