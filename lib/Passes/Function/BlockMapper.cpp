#include <llvm/IR/Instructions.h>

#include "Passes/Function/BlockMapper.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Util/Exceptions.h"

#include <utility>

namespace llvm {

    /*
     * Block Mapper Result
     */

    std::unordered_map<llvm::BasicBlock *, col::Block *> BMAResult::getRetBlock2ColBlock() {
        return retBlock2colBlock;
    }

    void BMAResult::addRetBlock2ColBlockEntry(llvm::BasicBlock &llvmBlock, col::Block &colBlock) {
        retBlock2colBlock.insert({&llvmBlock, &colBlock});
    }

    /*
     * Block Mapper (Analysis)
     */
    AnalysisKey BlockMapper::Key;


    BlockMapper::BlockMapper(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    BMAResult BlockMapper::run(Function &F, FunctionAnalysisManager &FAM) {
        result = BMAResult();
        llvm::BasicBlock &entryBlock = F.getEntryBlock();
        col::Block &funcBody = FAM.getResult<FunctionDeclarer>(F).getAssociatedColFuncBody();
        handleBlock(entryBlock, funcBody);
        return result;
    }

    void BlockMapper::handleBlock(BasicBlock &llvmBlock, col::Block &colBlock) {
        Instruction *termInstruction = llvmBlock.getTerminator();
        switch (Instruction::TermOps(termInstruction->getOpcode())) {
            case Instruction::Ret:
                handleReturnBlock(llvmBlock, colBlock);
                break;
            case Instruction::Br: {
                vcllvm::ErrorCollector::addError(
                        "Passes::Function::BlockMapper",
                        "Branch instructions not yet supported!"
                );
                // branchInst->isConditional() ? errs() << "i'm a branch!\n" : errs() << "i'm a jump!\n";
                break;
            }
            default:
                std::stringstream errorStream;
                errorStream << "Terminator instruction \"" << termInstruction->getName().str() << "\" in function \"" <<
                            termInstruction->getFunction()->getName().str() << "\" not supported";
                vcllvm::ErrorCollector::addError("Passes::Function::BlockMapper", errorStream.str());
        }
    }

    void BlockMapper::handleReturnBlock(BasicBlock &llvmBlock, col::Block &colBlock) {
        result.addRetBlock2ColBlockEntry(llvmBlock, colBlock);
    }

}