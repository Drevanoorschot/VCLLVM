#include "Transform/Instruction/BinaryOpTransform.h"


#include "Transform/Origin/OriginProvider.h"
#include "Transform/Transform.h"
#include "Util/Exceptions.h"

namespace llvm2Col {

    void transformBinaryOp(llvm::Instruction &llvmInstruction,
                           col::Block &colBlock,
                           vcllvm::FunctionCursor &funcCursor) {
        // add var decl to the scope
        col::Variable *varDecl = funcCursor.getFunctionScope().add_locals();
        transformAndSetType(*llvmInstruction.getType(), *varDecl->mutable_t());
        setColNodeId(varDecl);
        // add var decl to function cursor var look up table
        funcCursor.addVariableMapEntry(llvmInstruction, *varDecl);
        // add variable assignment to the COL block
        col::Assign *assignment = colBlock.add_statements()->mutable_assign();
        // set origin of assignment
        assignment->set_origin(llvm2Col::generateAssignmentOrigin(llvmInstruction));
        // set target to refer to var decl
        assignment->mutable_target()->mutable_local()->mutable_ref()->set_index(varDecl->id());
        switch (llvm::Instruction::BinaryOps(llvmInstruction.getOpcode())) {
            case llvm::Instruction::Add: {
                col::Plus &expr = transformAdd(*assignment);
                expr.set_origin(generateBinExprOrigin(llvmInstruction));
                transformOperands(llvmInstruction, expr, funcCursor);
                break;
            }
            case llvm::Instruction::Sub: {
                col::Minus &expr = transformSub(*assignment);
                expr.set_origin(generateBinExprOrigin(llvmInstruction));
                transformOperands(llvmInstruction, expr, funcCursor);
                break;
            }
            case llvm::Instruction::Mul: {
                col::Mult &expr = transformMul(*assignment);
                expr.set_origin(generateBinExprOrigin(llvmInstruction));
                transformOperands(llvmInstruction, expr, funcCursor);
                break;
            }
            case llvm::Instruction::SDiv:
            case llvm::Instruction::UDiv: {
                col::FloorDiv &expr = transformDiv(*assignment);
                expr.set_origin(generateBinExprOrigin(llvmInstruction));
                transformOperands(llvmInstruction, expr, funcCursor);
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

    void transformOperands(llvm::Instruction &llvmInstruction,
                           auto &colBinExpr,
                           vcllvm::FunctionCursor &funcCursor) {
        // transform left operand
        col::Expr *lExpr = colBinExpr.mutable_left();
        llvm2Col::transformAndSetExpr(
                funcCursor, *llvmInstruction.getOperand(0),
                *lExpr);
        lExpr->set_origin(generateOperandOrigin(llvmInstruction, *llvmInstruction.getOperand(0)));
        // transform right operand
        col::Expr *rExpr = colBinExpr.mutable_right();
        llvm2Col::transformAndSetExpr(
                funcCursor, *llvmInstruction.getOperand(1),
                *rExpr);
        rExpr->set_origin(generateOperandOrigin(llvmInstruction, *llvmInstruction.getOperand(1)));
    }

    col::Plus &transformAdd(col::Assign &assignment) {
        col::Plus *plusExpr = assignment.mutable_value()->mutable_plus();
        return *plusExpr;
    }

    col::Minus &transformSub(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_minus();
    }

    col::Mult &transformMul(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_mult();
    }

    col::FloorDiv &transformDiv(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_floor_div();
    }
}