#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Instructions.h>

#include "Passes/Function/BlockMapper.h"
#include "Passes/Function/FunctionDeclarer.h"

#include "Util/Exceptions.h"
#include <utility>

namespace vcllvm {
    /*
     * Block Mapper Result
     */

    std::unordered_map<llvm::BasicBlock *, col::Block *> BMAResult::getLlvmBlock2ColBlock() {
        return llvmBlock2ColBlock;
    }

    void BMAResult::addLlvmBlock2ColBlockEntry(llvm::BasicBlock &llvmBlock,
                                               col::Block &colBlock) {
        llvmBlock2ColBlock.insert({&llvmBlock, &colBlock});
    }

    /*
     * Block Mapper (Analysis)
     */
    AnalysisKey BlockMapper::Key;


    BlockMapper::BlockMapper(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    BMAResult BlockMapper::run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
        result = BMAResult();
        llvm::BasicBlock &entryBlock = F.getEntryBlock();
        col::Block *funcBody = FAM.getResult<FunctionDeclarer>(F).getAssociatedScopedColFuncBody().block;
        handleBlock(entryBlock, *funcBody, FAM);
        return result;
    }

    void BlockMapper::handleBlock(BasicBlock &llvmBlock,
                                  col::Block &colBlock,
                                  llvm::FunctionAnalysisManager &FAM) {
        Instruction *termInstruction = llvmBlock.getTerminator();
        switch (Instruction::TermOps(termInstruction->getOpcode())) {
            case Instruction::Ret:
                handleReturnBlock(llvmBlock, colBlock);
                break;
            case Instruction::Br:
                if (FAM.getResult<LoopAnalysis>(*termInstruction->getFunction()).isLoopHeader(&llvmBlock)) {
                    handleLoop(llvmBlock, colBlock, FAM);
                } else if (termInstruction->getNumOperands() == 2) {
                    handleBranch(llvmBlock, colBlock, FAM);
                } else {
                    handleJump(llvmBlock, colBlock, FAM);
                }
                break;
            default:
                std::stringstream errorStream;
                errorStream << "Terminator instruction \"" << termInstruction->getName().str() << "\" in function \"" <<
                            termInstruction->getFunction()->getName().str() << "\" not supported";
                vcllvm::ErrorReporter::addError("Passes::Function::BlockMapper", errorStream.str());
        }
    }

    void BlockMapper::handleReturnBlock(BasicBlock &llvmBlock, col::Block &colBlock) {
        handleBlock2Block(llvmBlock, colBlock);
    }

    void BlockMapper::handleLoop(BasicBlock &llvmBlock,
                                 col::Block &colBlock,
                                 FunctionAnalysisManager &FAM) {
        if (llvmBlock.getTerminator()->getNumOperands() == 1) {
            std::stringstream errorStream;
            llvm::errs() << "block \"" << llvmBlock.getName()
                         << "has a single branch and is header, "
                            "likely indicating an infinite loop (which is unsupported)\n";
//            errorStream << "Branch instruction in block " << llvmBlock.
//            vcllvm::ErrorReporter::addError("Passes::Function::BlockMapper")
        }
    }

    void BlockMapper::handleBranch(BasicBlock &llvmBlock,
                                   col::Block &colBlock,
                                   FunctionAnalysisManager &FAM) {
        // TODO implement
    }

    void BlockMapper::handleJump(BasicBlock &llvmBlock,
                                 col::Block &colBlock,
                                 FunctionAnalysisManager &FAM) {
        handleBlock2Block(llvmBlock, colBlock);
        llvm::Loop *loop = FAM.getResult<LoopAnalysis>(*llvmBlock.getTerminator()->getFunction()).getLoopFor(
                &llvmBlock);
        // handle loop exit (can go to parent loop or further, how to determine this?)
        if (loop != nullptr && loop->isLoopExiting(&llvmBlock)) {


        } else if (loop != nullptr && loop->isLoopLatch(&llvmBlock)) { // end of the loop body, do nothing

        } else { // go on to the next block

        }
    }

    void BlockMapper::handleBlock2Block(BasicBlock &llvmBlock, col::Block &colBlock) {
        result.addLlvmBlock2ColBlockEntry(llvmBlock, colBlock);
    }


}