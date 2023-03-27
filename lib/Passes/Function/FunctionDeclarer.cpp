#include "Passes/Function/FunctionDeclarer.h"
#include "Transform/Transform.h"
#include "Util/Exceptions.h"


namespace vcllvm {
    using namespace llvm;

    /*
     * Function Declarer Result
     */

    FDResult::FDResult(col::LlvmFunctionDefinition &colFuncDef, ColScopedFuncBody associatedScopedColFuncBody) :
            associatedColFuncDef(colFuncDef),
            associatedScopedColFuncBody(associatedScopedColFuncBody) {}

    col::LlvmFunctionDefinition &FDResult::getAssociatedColFuncDef() {
        return associatedColFuncDef;
    }

    ColScopedFuncBody FDResult::getAssociatedScopedColFuncBody() {
        return associatedScopedColFuncBody;
    }

    void FDResult::addFuncArgMapEntry(Argument &llvmArg, col::Variable &colArg) {
        funcArgMap.insert({&llvmArg, &colArg});
    }

    col::Variable &FDResult::getFuncArgMapEntry(Argument &arg) {
        return *funcArgMap.at(&arg);
    }


    /*
     * Function Declarer (Analysis)
     */
    AnalysisKey FunctionDeclarer::Key;

    FunctionDeclarer::FunctionDeclarer(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    FDResult FunctionDeclarer::run(Function &F, FunctionAnalysisManager &FAM) {
        // create llvmFuncDef declaration in buffer
        col::GlobalDeclaration *llvmFuncDefDecl = pProgram->add_declarations();
        // generate id
        llvm2Col::setColNodeId(llvmFuncDefDecl);
        col::LlvmFunctionDefinition *llvmFuncDef = llvmFuncDefDecl->mutable_llvm_function_definition();
        // add body block + scope
        ColScopedFuncBody funcScopedBody{};
        funcScopedBody.scope = llvmFuncDef->mutable_body()->mutable_scope();
        funcScopedBody.block = funcScopedBody.scope->mutable_body()->mutable_block();
        FDResult result = FDResult(*llvmFuncDef, funcScopedBody);
        // set args (if present)
        for (llvm::Argument &llvmArg: F.args()) {
            // set in buffer
            col::Variable *colArg = llvmFuncDef->add_args();
            llvm2Col::setColNodeId(colArg);
            try {
                llvm2Col::convertAndSetType(*llvmArg.getType(), *colArg->mutable_t());
            } catch (vcllvm::UnsupportedTypeException &e) {
                std::stringstream errorStream;
                errorStream << e.what() << " in argument #" << llvmArg.getArgNo() << " of function \""
                            << F.getName().str() << "\"";
                vcllvm::ErrorReporter::addError("Passes::Function::FunctionDeclarer", errorStream.str());
            }
            // add args mapping to result
            result.addFuncArgMapEntry(llvmArg, *colArg);
        }
        return result;
    }

    /*
     * Function Declarer Pass
     */
    FunctionDeclarerPass::FunctionDeclarerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses FunctionDeclarerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        FDResult result = FAM.getResult<FunctionDeclarer>(F);
        col::LlvmFunctionDefinition &colFunction = result.getAssociatedColFuncDef();
        // complete the procedure declaration in proto buffer
        // set return type in protobuf of function
        try {
            llvm2Col::convertAndSetType(*F.getReturnType(), *colFunction.mutable_return_type());
        } catch (vcllvm::UnsupportedTypeException &e) {
            std::stringstream errorStream;
            errorStream << e.what() << " in return type of function \"" << F.getName().str() << "\"";
            vcllvm::ErrorReporter::addError("Passes::Function::FunctionDeclarer", errorStream.str());
        }
        //TODO body (separate pass)
        //TODO contract (separate pass)
        return PreservedAnalyses::all();
    }
}
