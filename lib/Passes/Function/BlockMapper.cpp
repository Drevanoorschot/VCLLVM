#include <llvm/IR/Instructions.h>

#include "Passes/Function/BlockMapper.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Util/Exceptions.h"

#include <utility>

namespace vcllvm {
    using namespace llvm;
    /*
     * Block Mapper Result
     */

    std::unordered_map<llvm::BasicBlock *, llvm2Col::ColScopedBlock> BMAResult::getRetBlock2ColScopedBlock() {
        return retBlock2ColScopedBlock;
    }

    void BMAResult::addRetBlock2ColScopedBlockEntry(llvm::BasicBlock &llvmBlock, llvm2Col::ColScopedBlock colScopedBlock) {
        retBlock2ColScopedBlock.insert({&llvmBlock, colScopedBlock});
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
        llvm2Col::ColScopedBlock funcScopedBody = FAM.getResult<FunctionDeclarer>(F).getAssociatedScopedColFuncBody();
        handleBlock(entryBlock, funcScopedBody);
        return result;
    }

    void BlockMapper::handleBlock(BasicBlock &llvmBlock, llvm2Col::ColScopedBlock colScopedBlock) {
        Instruction *termInstruction = llvmBlock.getTerminator();
        switch (Instruction::TermOps(termInstruction->getOpcode())) {
            case Instruction::Ret:
                handleReturnBlock(llvmBlock, colScopedBlock);
                break;
            case Instruction::Br: {
                vcllvm::ErrorReporter::addError(
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
                vcllvm::ErrorReporter::addError("Passes::Function::BlockMapper", errorStream.str());
        }
    }

    void BlockMapper::handleReturnBlock(BasicBlock &llvmBlock, llvm2Col::ColScopedBlock colScopedBlock) {
        result.addRetBlock2ColScopedBlockEntry(llvmBlock, colScopedBlock);
    }

}