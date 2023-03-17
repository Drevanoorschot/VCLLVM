#include <llvm/IR/InstrTypes.h>

#include "Passes/Function/BlockMapper.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Passes/Function/FunctionInstructionTransformer.h"
#include "Util/Exceptions.h"

#include <utility>

namespace vcllvm {

    FunctionCursor::FunctionCursor(BMAResult bmaResult) :
            bmaResult(std::move(bmaResult)) {}

    BMAResult &FunctionCursor::getBMAResult() {
        return bmaResult;
    }

    void FunctionCursor::addVariableMapEntry(LLVMVar llvmVar, COLVar colVar) {
        variableMap.insert({llvmVar, colVar});
    }

    COLVar FunctionCursor::getVariableMapEntry(LLVMVar llvmVar) {
        return variableMap.at(llvmVar);
    }

    FunctionCursor::FunctionCursor() = default;

    FunctionInstructionTransformerPass::FunctionInstructionTransformerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses FunctionInstructionTransformerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        // initialise function cursor
        functionCursor = FunctionCursor(FAM.getResult<BlockMapper>(F));
        // add function arguments to variable look up table
        for (llvm::Argument &A: F.args()) {
            functionCursor.addVariableMapEntry(&A, &FAM.getResult<FunctionDeclarer>(F).getFuncArgMapEntry(A));
        }
        // fetch entry block
        llvm::BasicBlock &entryBlock = F.getEntryBlock();
        transformBlock(entryBlock);
        return PreservedAnalyses::all();
    }

    void FunctionInstructionTransformerPass::transformBlock(llvm::BasicBlock &llvmBlock) {
        Instruction *termInstruction = llvmBlock.getTerminator();
        switch (Instruction::TermOps(termInstruction->getOpcode())) {
            case Instruction::Ret:
                transformRetBlock(llvmBlock);
                break;
            case Instruction::Br:
                vcllvm::ErrorCollector::addError(
                        "Passes::Function::BlockMapper",
                        "Branch instructions not yet supported!"
                );
                break;
            default:
                std::stringstream errorStream;
                errorStream << "Terminator instruction \"" << termInstruction->getName().str() << "\" in function \"" <<
                            termInstruction->getFunction()->getName().str() << "\" not supported";
                vcllvm::ErrorCollector::addError("Passes::Function::FunctionInstructionTransformer", errorStream.str());
        }
    }

    void FunctionInstructionTransformerPass::transformRetBlock(BasicBlock &llvmBlock) {
        // fetch related COL block.
        col::Block *colBlock = functionCursor.getBMAResult().getRetBlock2ColBlock().at(&llvmBlock);
        //TODO foreach instruction
        for (auto &I: llvmBlock.getInstList()) {
            //TODO upscope all contract metadata aliases
            //TODO Add instruction with created COL node to look up table
        }
    }
}
