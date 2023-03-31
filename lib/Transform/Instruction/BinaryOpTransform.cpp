#include "Transform/Instruction/BinaryOpTransform.h"
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
        // set target to refer to var decl
        assignment->mutable_target()->mutable_local()->mutable_ref()->set_index(varDecl->id());
        switch (llvm::Instruction::BinaryOps(llvmInstruction.getOpcode())) {
            case llvm::Instruction::Add: {
                col::Plus &plusExpr = transformAdd(*assignment);
                transformOperands(plusExpr, llvmInstruction, funcCursor);
                break;
            }
            case llvm::Instruction::Sub: {
                col::Minus &minExpr = transformSub(*assignment);
                transformOperands(minExpr, llvmInstruction, funcCursor);
            }
            case llvm::Instruction::Mul: {
                col::Mult &mulExpr = transformMul(*assignment);
                transformOperands(mulExpr, llvmInstruction, funcCursor);
            }
            case llvm::Instruction::SDiv:
            case llvm::Instruction::UDiv: {
                col::Div &divExpr = transformDiv(*assignment);
                transformOperands(divExpr, llvmInstruction, funcCursor);
            }
            default:
                std::stringstream errorStream;
                errorStream << "Unsupported operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                            << llvmInstruction.getFunction()->getName().str();
                vcllvm::ErrorReporter::addError("Util::Transform::Instruction::BinaryOp", errorStream.str());
                return;
        }
    }

    void transformOperands(auto &colBinExpr,
                           llvm::Instruction &llvmInstruction,
                           vcllvm::FunctionCursor &funcCursor) {
        // transform left operand
        llvm2Col::transformAndSetExpr(
                funcCursor, *llvmInstruction.getOperand(0),
                *colBinExpr.mutable_left());
        // transform right operand
        llvm2Col::transformAndSetExpr(
                funcCursor, *llvmInstruction.getOperand(1),
                *colBinExpr.mutable_right());
    }

    col::Plus &transformAdd(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_plus();
    }

    col::Minus &transformSub(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_minus();
    }

    col::Mult &transformMul(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_mult();
    }

    col::Div &transformDiv(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_div();
    }


}