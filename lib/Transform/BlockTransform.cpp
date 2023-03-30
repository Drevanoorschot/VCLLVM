#include "Transform/BlockTransform.h"
#include "Transform/Instruction/BinaryOpTransform.h"
#include "Transform/Instruction/UnaryOpTransform.h"
#include "Transform/Instruction/MemoryOpTransform.h"
#include "Transform/Instruction/FuncletPadOpTransform.h"
#include "Transform/Instruction/OtherOpTransform.h"
#include "Util/Exceptions.h"
#include "Transform/Transform.h"

namespace llvm2Col {


    void transformLlvmBlock(vcllvm::FunctionCursor &functionCursor,
                            col::Block &currentColBody,
                            llvm::BasicBlock &llvmBlock) {
        for (auto &I: llvmBlock.getInstList()) {
            if (I.isTerminator()) {
                break;
            }
            convertNonTermInstruction(I, currentColBody, functionCursor);
        }
        llvm::Instruction *termInstruction = llvmBlock.getTerminator();
        switch (llvm::Instruction::TermOps(termInstruction->getOpcode())) {
            case llvm::Instruction::Ret: {
                col::Expr *returnExpr = currentColBody.add_statements()
                        ->mutable_return_()
                        ->mutable_result();
                llvm2Col::transformAndSetExpr(
                        *termInstruction->getOperand(0),
                        *returnExpr,
                        functionCursor
                );
                break;
            }
            case llvm::Instruction::Br:
                break;
            default:
                std::stringstream errorStream;
                errorStream << "Terminator instruction \"" << termInstruction->getName().str() << "\" in function \"" <<
                            termInstruction->getFunction()->getName().str() << "\" not supported";
                vcllvm::ErrorReporter::addError("Transform::BlockTransform", errorStream.str());
        }
    }

    void convertNonTermInstruction(llvm::Instruction &llvmInstruction,
                                   col::Block &colBlock,
                                   vcllvm::FunctionCursor &funcCursor) {
        u_int32_t opCode = llvmInstruction.getOpcode();
        if (llvm::Instruction::TermOpsBegin <= opCode && opCode <= llvm::Instruction::TermOpsEnd) {
            vcllvm::ErrorReporter::addError("Transform::BlockTransform",
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
            vcllvm::ErrorReporter::addError("Transform::BlockTransform", errorStream.str());
        }
    }
}
