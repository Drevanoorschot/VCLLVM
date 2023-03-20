#include <llvm/IR/InstrTypes.h>

#include "Passes/Function/BlockMapper.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Passes/Function/FunctionInstructionTransformer.h"
#include "Util/Conversion/Instruction/UnaryOpConversion.h"
#include "Util/Conversion/Instruction/BinaryOpConversion.h"
#include "Util/Conversion/Instruction/MemoryOpConversion.h"
#include "Util/Conversion/Instruction/FuncletPadOpConversion.h"
#include "Util/Conversion/Instruction/OtherOpConversion.h"
#include "Util/Exceptions.h"

#include <utility>

namespace vcllvm {

    FunctionCursor::FunctionCursor(BMAResult bmaResult) :
            bmaResult(std::move(bmaResult)) {}

    BMAResult &FunctionCursor::getBMAResult() {
        return bmaResult;
    }

    void FunctionCursor::addVariableMapEntry(llvm::Value &llvmValue, col::Variable &colVar) {
        variableMap.insert({&llvmValue, &colVar});
    }

    col::Variable &FunctionCursor::getVariableMapEntry(llvm::Value &llvmValue) {
        return *variableMap.at(&llvmValue);
    }

    FunctionCursor::FunctionCursor() = default;

    FunctionInstructionTransformerPass::FunctionInstructionTransformerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses FunctionInstructionTransformerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        // initialise function cursor
        functionCursor = FunctionCursor(FAM.getResult<BlockMapper>(F));
        // add function arguments to variable look up table
        for (llvm::Argument &A: F.args()) {
            functionCursor.addVariableMapEntry(A, FAM.getResult<FunctionDeclarer>(F).getFuncArgMapEntry(A));
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
                vcllvm::ErrorReporter::addError(
                        "Passes::Function::BlockMapper",
                        "Branch instructions not yet supported!"
                );
                break;
            default:
                std::stringstream errorStream;
                errorStream << "Terminator instruction \"" << termInstruction->getName().str() << "\" in function \"" <<
                            termInstruction->getFunction()->getName().str() << "\" not supported";
                vcllvm::ErrorReporter::addError("Passes::Function::FunctionInstructionTransformer", errorStream.str());
        }
    }

    void FunctionInstructionTransformerPass::transformRetBlock(BasicBlock &llvmBlock) {
        // fetch related COL block.
        llvm2Col::ColScopedBlock colScopedBlock = functionCursor.getBMAResult().getRetBlock2ColScopedBlock().at(
                &llvmBlock);
        //TODO foreach instruction
        for (auto &I: llvmBlock.getInstList()) {
            if (I.isTerminator()) {
                break;
            }
            convertNonTermInstruction(I, colScopedBlock, functionCursor);
            //TODO upscope all contract metadata aliases
        }
    }

    void convertNonTermInstruction(llvm::Instruction &llvmInstruction,
                                   llvm2Col::ColScopedBlock colScopedBlock,
                                   vcllvm::FunctionCursor &funcCursor) {
        u_int32_t opCode = llvmInstruction.getOpcode();
        if (llvm::Instruction::TermOpsBegin <= opCode && opCode <= llvm::Instruction::TermOpsEnd) {
            vcllvm::ErrorReporter::addError("Passes::Function::FunctionInstructionTransformer",
                                            "Wrong method call to handle terminator instructions!");
        } else if (llvm::Instruction::BinaryOpsBegin <= opCode && opCode < llvm::Instruction::BinaryOpsEnd) {
            llvm2Col::convertBinaryOp(llvmInstruction, colScopedBlock, funcCursor);
        } else if (llvm::Instruction::UnaryOpsBegin <= opCode && opCode < llvm::Instruction::UnaryOpsEnd) {
            llvm2Col::convertUnaryOp(llvmInstruction, colScopedBlock, funcCursor);
        } else if (llvm::Instruction::MemoryOpsBegin <= opCode && opCode < llvm::Instruction::MemoryOpsEnd) {
            llvm2Col::convertMemoryOp(llvmInstruction, colScopedBlock, funcCursor);
        } else if (llvm::Instruction::FuncletPadOpsBegin <= opCode && opCode < llvm::Instruction::FuncletPadOpsEnd) {
            llvm2Col::convertFuncletPadOp(llvmInstruction, colScopedBlock, funcCursor);
        } else if (llvm::Instruction::OtherOpsBegin <= opCode && opCode < llvm::Instruction::OtherOpsEnd) {
            llvm2Col::convertOtherOp(llvmInstruction, colScopedBlock, funcCursor);
        } else {
            std::stringstream errorStream;
            errorStream << "Unknown operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                        << llvmInstruction.getFunction()->getName().str() << "\"";
            vcllvm::ErrorReporter::addError("Passes::Function::FunctionInstructionTransformer", errorStream.str());
        }
    }
}
