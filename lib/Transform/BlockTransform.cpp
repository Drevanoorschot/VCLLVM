#include "llvm/IR/CFG.h"

#include "Transform/BlockTransform.h"
#include "Transform/Instruction/TermOpTransform.h"
#include "Transform/Instruction/BinaryOpTransform.h"
#include "Transform/Instruction/UnaryOpTransform.h"
#include "Transform/Instruction/MemoryOpTransform.h"
#include "Transform/Instruction/FuncletPadOpTransform.h"
#include "Transform/Instruction/OtherOpTransform.h"
#include "Util/Exceptions.h"

namespace llvm2Col {
    void transformLlvmBlock(llvm::BasicBlock &llvmBlock, vcllvm::FunctionCursor &functionCursor) {
        col::Block &colBlock = functionCursor.getOrSetLlvmBlock2LabeledColBlockEntry(llvmBlock).block;
        for (auto *B: llvm::predecessors(&llvmBlock)) {
            if (!functionCursor.isVisited(*B)) return;
        }
        if (functionCursor.getLoopInfo().isLoopHeader(&llvmBlock)) {
            transformLoop(llvmBlock, functionCursor);
            return;
        }
        for (auto &I: llvmBlock.getInstList()) {
            transformInstruction(functionCursor, I, colBlock);
        }
    }

    void transformInstruction(vcllvm::FunctionCursor &funcCursor,
                              llvm::Instruction &llvmInstruction,
                              col::Block &colBodyBlock) {
        u_int32_t opCode = llvmInstruction.getOpcode();
        if (llvm::Instruction::TermOpsBegin <= opCode && opCode < llvm::Instruction::TermOpsEnd) {
            llvm2Col::transformTermOp(llvmInstruction, colBodyBlock, funcCursor);
        } else if (llvm::Instruction::BinaryOpsBegin <= opCode && opCode < llvm::Instruction::BinaryOpsEnd) {
            llvm2Col::transformBinaryOp(llvmInstruction, colBodyBlock, funcCursor);
        } else if (llvm::Instruction::UnaryOpsBegin <= opCode && opCode < llvm::Instruction::UnaryOpsEnd) {
            llvm2Col::transformUnaryOp(llvmInstruction, colBodyBlock, funcCursor);
        } else if (llvm::Instruction::MemoryOpsBegin <= opCode && opCode < llvm::Instruction::MemoryOpsEnd) {
            llvm2Col::transformMemoryOp(llvmInstruction, colBodyBlock, funcCursor);
        } else if (llvm::Instruction::FuncletPadOpsBegin <= opCode && opCode < llvm::Instruction::FuncletPadOpsEnd) {
            llvm2Col::transformFuncletPadOp(llvmInstruction, colBodyBlock, funcCursor);
        } else if (llvm::Instruction::OtherOpsBegin <= opCode && opCode < llvm::Instruction::OtherOpsEnd) {
            llvm2Col::transformOtherOp(llvmInstruction, colBodyBlock, funcCursor);
        } else {
            std::stringstream errorStream;
            errorStream << "Unknown operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                        << llvmInstruction.getFunction()->getName().str() << "\"";
            vcllvm::ErrorReporter::addError("Transform::BlockTransform", errorStream.str());
        }
    }

    void transformLoop(llvm::BasicBlock &llvmBlock, vcllvm::FunctionCursor &functionCursor) {
        std::stringstream errorStream;
        errorStream << "Loops are unsupported. Loop detected in function \"" << llvmBlock.getParent()->getName().str()
                    << "\"\n";
        vcllvm::ErrorReporter::addError("Transform::BlockTransform", errorStream.str());
    }
}
