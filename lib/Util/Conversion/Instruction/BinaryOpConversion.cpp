#include "Util/Conversion/Instruction/BinaryOpConversion.h"
#include "Util/Exceptions.h"

namespace llvm2Col {

    void convertBinaryOp(llvm::Instruction &llvmInstruction,
                         ColScopedBlock colScopedBlock,
                         vcllvm::FunctionCursor &funcCursor) {
        // add var decl to the scope
        col::Variable *varDecl = colScopedBlock.scope->add_locals();
        convertAndSetType(*llvmInstruction.getType(), *varDecl->mutable_t());
        setColNodeId(varDecl);
        // add var decl to function cursor var look up table
        funcCursor.addVariableMapEntry(llvmInstruction, *varDecl);
        // add variable assignment to the COL block
        col::Assign *assignment = colScopedBlock.block->add_statements()->mutable_assign();
        // set target to refer to var decl
        assignment->mutable_target()->mutable_local()->mutable_ref()->set_index(varDecl->id());
        switch (llvm::Instruction::BinaryOps(llvmInstruction.getOpcode())) {
            case llvm::Instruction::Add: {
                col::Plus &plusExpr = convertAdd(*assignment);
                convertOperands(plusExpr, llvmInstruction, colScopedBlock, funcCursor);
                break;
            }
            default:
                std::stringstream errorStream;
                errorStream << "Unsupported operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                            << llvmInstruction.getFunction()->getName().str();
                vcllvm::ErrorReporter::addError("Util::Conversion::Instruction::BinaryOp", errorStream.str());
                return;
        }
    }

    col::Plus &convertAdd(col::Assign &assignment) {
        return *assignment.mutable_value()->mutable_plus();
    }

    void convertOperands(auto &binExpr,
                         llvm::Instruction &llvmInstruction,
                         ColScopedBlock colScopedBlock,
                         vcllvm::FunctionCursor &funcCursor) {
        // left hand side...
        col::Variable leftColVar = funcCursor.getVariableMapEntry(*llvmInstruction.getOperand(0));
        binExpr.mutable_left()->mutable_local()->mutable_ref()->set_index(leftColVar.id());
        // ...right hand side
        col::Variable rightColVar = funcCursor.getVariableMapEntry(*llvmInstruction.getOperand(1));
        binExpr.mutable_right()->mutable_local()->mutable_ref()->set_index(rightColVar.id());
    }


}