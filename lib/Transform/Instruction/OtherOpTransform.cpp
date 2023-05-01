#include "Transform/Instruction/OtherOpTransform.h"

#include "Transform/BlockTransform.h"
#include "Transform/Transform.h"
#include "Util/Exceptions.h"

namespace llvm2Col {
    const std::string SOURCE_LOC = "Transform::Instruction::OtherOp";

    void transformOtherOp(llvm::Instruction &llvmInstruction,
                          col::Block &colBlock,
                          vcllvm::FunctionCursor &funcCursor) {
        switch(llvm::Instruction::OtherOps(llvmInstruction.getOpcode())) {
            case llvm::Instruction::PHI:
                transformPhi(llvm::cast<llvm::PHINode>(llvmInstruction), colBlock, funcCursor);
                break;
            default:
                reportUnsupportedOperatorError(SOURCE_LOC, llvmInstruction);
        }
    }

    void transformPhi(llvm::PHINode &phiInstruction,
                      col::Block &colBlock,
                      vcllvm::FunctionCursor &funcCursor) {
        col::Variable &varDecl = funcCursor.declareVariable(phiInstruction);
        for(auto &B : phiInstruction.blocks()) {
            // add assignment of the variable to target block
            col::Block &targetBlock = funcCursor.getOrSetLlvmBlock2LabeledColBlockEntry(*B).block;
            col::Assign &assignment = funcCursor.createAssignmentInFunction(phiInstruction, targetBlock, varDecl);
            // assign correct value by looking at the value-block pair of phi instruction.
            col::Expr *value = assignment.mutable_value();
            llvm2Col::transformAndSetExpr(funcCursor,
                                          phiInstruction,
                                          *phiInstruction.getIncomingValueForBlock(B),
                                          *value);
        }
    }
}