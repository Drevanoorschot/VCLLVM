//
// Created by dre on 3/9/23.
//

#include "Passes/Function/FunctionContractDeclarer.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Util/Constants.h"
#include "Util/Exceptions.h"


namespace llvm {
    namespace col = vct::col::serialize;

    /*
     * Function Contract Declarer Result
     */

    FDCResult::FDCResult(vct::col::serialize::LlvmFunctionContract *colFuncContract) :
            associatedColFuncContract(colFuncContract) {}

    col::LlvmFunctionContract *FDCResult::getAssociatedColFuncContract() {
        return associatedColFuncContract;
    }

    /*
     * Function Contract Declarer (Analysis)
     */

    AnalysisKey FunctionContractDeclarer::Key;


    FunctionContractDeclarer::FunctionContractDeclarer(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    FunctionContractDeclarer::Result FunctionContractDeclarer::run(Function &F, FunctionAnalysisManager &FAM) {
        // fetch relevant function from the Function Declarer
        FDResult fdResult = FAM.getResult<FunctionDeclarer>(F);
        col::LlvmFunctionDefinition *colFunction = fdResult.getAssociatedColFuncDef();
        // set a contract in the buffer as well as make and return a result object
        return FDCResult(colFunction->mutable_contract());
    }

    /*
     * Function Contract Declarer Pass
     */

    FunctionContractDeclarerPass::FunctionContractDeclarerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses FunctionContractDeclarerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        // get col contract
        FDCResult result = FAM.getResult<FunctionContractDeclarer>(F);
        col::LlvmFunctionContract *colContract = result.getAssociatedColFuncContract();
        // check if contract keyword is present
        if (!F.hasMetadata(vcllvm::constants::METADATA_CONTRACT_KEYWORD)) {
            // empty contract
            colContract->set_value("");
            return PreservedAnalyses::all();
        }
        // concatenate all contract lines with new lines
        MDNode *contractMDNode = F.getMetadata(vcllvm::constants::METADATA_CONTRACT_KEYWORD);
        std::stringstream contractStream;
        for (int i = 0; i < contractMDNode->getNumOperands(); i++) {
            auto contractLine = dyn_cast<MDString>(contractMDNode->getOperand(i));
            if (contractLine == nullptr) {
                std::stringstream errorStream;
                errorStream << "Unable to cast contract string #" << i + 1 << " of function \""
                            << F.getName().str() << "\" to string type";
                vcllvm::ErrorCollector::addError("Passes::Function::FunctionContractDeclarer", errorStream.str());
                break;
            }
            contractStream << contractLine->getString().str() << '\n';
        }
        colContract->set_value(contractStream.str());
        return PreservedAnalyses::all();
    }
}