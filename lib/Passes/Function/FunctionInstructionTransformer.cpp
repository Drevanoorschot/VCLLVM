#include <llvm/IR/InstrTypes.h>

#include "Passes/Function/BlockMapper.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Passes/Function/FunctionInstructionTransformer.h"
#include "Conversion/Instruction/UnaryOpConversion.h"
#include "Conversion/Instruction/BinaryOpConversion.h"
#include "Conversion/Instruction/MemoryOpConversion.h"
#include "Conversion/Instruction/FuncletPadOpConversion.h"
#include "Conversion/Instruction/OtherOpConversion.h"
#include "Util/Exceptions.h"

#include <utility>

namespace vcllvm {

    FunctionCursor::FunctionCursor(BMAResult bmaResult, col::Scope &functionScope) :
            bmaResult(std::move(bmaResult)), functionScope(&functionScope) {}

    BMAResult &FunctionCursor::getBmaResult() {
        return bmaResult;
    }

    col::Scope &FunctionCursor::getFunctionScope() {
        return *functionScope;
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
        functionCursor = FunctionCursor(FAM.getResult<BlockMapper>(F),
                                        *FAM.getResult<FunctionDeclarer>(F).getAssociatedScopedColFuncBody().scope);
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
        col::Block *colBlock = functionCursor
                .getBmaResult()
                .getLlvmBlock2ColBlock()
                .at(&llvmBlock);
        //TODO foreach instruction
        for (auto &I: llvmBlock.getInstList()) {
            if (I.isTerminator()) {
                // handle return instruction
                colBlock->add_statements()
                        ->mutable_return_()
                        ->mutable_result()
                        ->mutable_local()
                        ->mutable_ref()
                        ->set_index(functionCursor.getVariableMapEntry(*I.getOperand(0)).id());
            } else {
                convertNonTermInstruction(I, *colBlock, functionCursor);
                //TODO upscope all contract metadata aliases
            }
        }
    }

    void convertNonTermInstruction(llvm::Instruction &llvmInstruction,
                                   col::Block &colBlock,
                                   vcllvm::FunctionCursor &funcCursor) {
        u_int32_t opCode = llvmInstruction.getOpcode();
        if (llvm::Instruction::TermOpsBegin <= opCode && opCode <= llvm::Instruction::TermOpsEnd) {
            vcllvm::ErrorReporter::addError("Passes::Function::FunctionInstructionTransformer",
                                            "Wrong method call to handle terminator instructions!");
        } else if (llvm::Instruction::BinaryOpsBegin <= opCode && opCode < llvm::Instruction::BinaryOpsEnd) {
            llvm2Col::convertBinaryOp(llvmInstruction, colBlock, funcCursor);
        } else if (llvm::Instruction::UnaryOpsBegin <= opCode && opCode < llvm::Instruction::UnaryOpsEnd) {
            llvm2Col::convertUnaryOp(llvmInstruction, colBlock, funcCursor);
        } else if (llvm::Instruction::MemoryOpsBegin <= opCode && opCode < llvm::Instruction::MemoryOpsEnd) {
            llvm2Col::convertMemoryOp(llvmInstruction, colBlock, funcCursor);
        } else if (llvm::Instruction::FuncletPadOpsBegin <= opCode && opCode < llvm::Instruction::FuncletPadOpsEnd) {
            llvm2Col::convertFuncletPadOp(llvmInstruction, colBlock, funcCursor);
        } else if (llvm::Instruction::OtherOpsBegin <= opCode && opCode < llvm::Instruction::OtherOpsEnd) {
            llvm2Col::convertOtherOp(llvmInstruction, colBlock, funcCursor);
        } else {
            std::stringstream errorStream;
            errorStream << "Unknown operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                        << llvmInstruction.getFunction()->getName().str() << "\"";
            vcllvm::ErrorReporter::addError("Passes::Function::FunctionInstructionTransformer", errorStream.str());
        }
    }
}
