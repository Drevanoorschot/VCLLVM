//
// Created by dre on 3/9/23.
//

#include "Passes/Function/PureAssigner.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Util/Constants.h"
#include "Util/Exceptions.h"

namespace llvm {

    PureAssignerPass::PureAssignerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses PureAssignerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        std::ostringstream errorStream;
        FDResult result = FAM.getResult<FunctionDeclarer>(F);
        col::LlvmFunctionDefinition *colFunction = result.getAssociatedColFuncDef();
        // check if pure keyword is present, else assume unpure function
        if (!F.hasMetadata(vcllvm::constants::METADATA_PURE_KEYWORD)) {
            colFunction->set_pure(false);
            return PreservedAnalyses::all();
        }
        // check if the 'pure' metadata has only 1 operand, else exit with error
        MDNode *pureMDNode = F.getMetadata(vcllvm::constants::METADATA_PURE_KEYWORD);
        if (pureMDNode->getNumOperands() != 1) {
            errorStream << "Expected 1 argument but got " << pureMDNode->getNumOperands();
            reportError(F, errorStream.str());
            return PreservedAnalyses::all();
        }
        // check if the only operand is of type 'i1', else exit with error
        auto *pureMDValue = cast<ValueAsMetadata>(pureMDNode->getOperand(0));
        if (!pureMDValue->getType()->isIntegerTy(1)) {
            errorStream << "MD node type must be of type \"i1\"";
            reportError(F, errorStream.str());
            return PreservedAnalyses::all();
        }
        // attempt down cast to ConstantInt (which shouldn't fail given previous checks)
        bool purity = cast<ConstantAsMetadata>(pureMDValue)->getValue()->isOneValue();
        colFunction->set_pure(purity);
        return PreservedAnalyses::all();
    }

    void reportError(Function &F, const std::string &explanation) {
        std::stringstream errorStream;
        errorStream << "Malformed Metadata node of type \"" << vcllvm::constants::METADATA_PURE_KEYWORD <<
                    "\" in function \"" << F.getName().str() << "\": " << explanation;
        vcllvm::ErrorCollector::addError("Passes::Function::PureAssigner", errorStream.str());
    }
}