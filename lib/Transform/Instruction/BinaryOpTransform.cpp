#include "Transform/Instruction/BinaryOpTransform.h"


#include "Transform/Origin/OriginProvider.h"
#include "Transform/Transform.h"
#include "Util/Exceptions.h"

namespace llvm2Col {

    void transformBinaryOp(llvm::Instruction &llvmInstruction,
                           col::Block &colBlock,
                           vcllvm::FunctionCursor &funcCursor) {
        col::Assign &assignment = funcCursor.createAssignmentInFunction(llvmInstruction, colBlock);
        switch (llvm::Instruction::BinaryOps(llvmInstruction.getOpcode())) {
            case llvm::Instruction::Add: {
                col::Plus &expr = *assignment.mutable_value()->mutable_plus();
                transformBinExpr(llvmInstruction, expr, funcCursor);
                break;
            }
            case llvm::Instruction::Sub: {
                col::Minus &expr = *assignment.mutable_value()->mutable_minus();
                transformBinExpr(llvmInstruction, expr, funcCursor);
                break;
            }
            case llvm::Instruction::Mul: {
                col::Mult &expr = *assignment.mutable_value()->mutable_mult();
                transformBinExpr(llvmInstruction, expr, funcCursor);
                break;
            }
            case llvm::Instruction::SDiv:
            case llvm::Instruction::UDiv: {
                col::FloorDiv &expr = *assignment.mutable_value()->mutable_floor_div();
                transformBinExpr(llvmInstruction, expr, funcCursor);
                break;
            }
            default:
                std::stringstream errorStream;
                errorStream << "Unsupported operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                            << llvmInstruction.getFunction()->getName().str() + '\"';
                vcllvm::ErrorReporter::addError("Util::Transform::Instruction::BinaryOp", errorStream.str());
                return;
        }
    }

    void transformBinExpr(llvm::Instruction &llvmInstruction,
                          auto &colBinExpr,
                          vcllvm::FunctionCursor &funcCursor) {
        // set origin of entire expression
        colBinExpr.set_origin(generateBinExprOrigin(llvmInstruction));
        // transform left operand
        col::Expr *lExpr = colBinExpr.mutable_left();
        llvm2Col::transformAndSetExpr(
                funcCursor, llvmInstruction, *llvmInstruction.getOperand(0),
                *lExpr);
        // transform right operand
        col::Expr *rExpr = colBinExpr.mutable_right();
        llvm2Col::transformAndSetExpr(
                funcCursor, llvmInstruction, *llvmInstruction.getOperand(1),
                *rExpr);
    }
}