//
// Created by dre on 3/9/23.
//

#include "Passes/Function/PureAssigner.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Util/Constants.h"

namespace llvm {
    namespace col = vct::col::serialize;

    PureAssignerPass::PureAssignerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses PureAssignerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        std::ostringstream errorStream;
        FDResult result = FAM.getResult<FunctionDeclarer>(F);
        col::LlvmFunctionDefinition *colFunction = result.getAssociatedColFuncDef();
        // Check if pure keyword is present, else assume unpure function
        if (!F.hasMetadata(vcllvm::constants::METADATA_PURE_KEYWORD)) {
            colFunction->set_pure(false);
            return PreservedAnalyses::all();
        }
        // Check if the 'pure' metadata has only 1 operand, else exit with error
        MDNode *pureMDNode = F.getMetadata(vcllvm::constants::METADATA_PURE_KEYWORD);
        if (pureMDNode->getNumOperands() != 1) {
            errorStream << "Expected 1 argument but got" << pureMDNode->getNumOperands();
            reportError(F, errorStream.str());
            exit(EXIT_FAILURE);
        }
        // check if the only operand is of type 'i1', else exit with error
        auto *pureMDValue = cast<ValueAsMetadata>(pureMDNode->getOperand(0));
        if (!pureMDValue->getType()->isIntegerTy(1)) {
            errorStream << "MD node type must be of type \"i1\"";
            reportError(F, errorStream.str());
            exit(EXIT_FAILURE);
        }
        // attempt down cast to ConstantInt
        bool purity = cast<ConstantAsMetadata>(pureMDValue)->getValue()->isOneValue();
        colFunction->set_pure(purity);
        errs() << F.getName() << ":" << colFunction->pure() << '\n';
        return PreservedAnalyses::all();
    }

    void reportError(Function &F, const std::string& explanation) {
        errs() << "Malformed Metadata node of type \"" << vcllvm::constants::METADATA_PURE_KEYWORD <<
               "\" in function \"" << F.getName() << "\": " << explanation << '\n';
    }
}