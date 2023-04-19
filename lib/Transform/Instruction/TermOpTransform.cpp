#include "Transform/Instruction/TermOpTransform.h"

#include "Transform/Transform.h"
#include "Transform/BlockTransform.h"
#include "Util/Exceptions.h"
#include "Origin/OriginProvider.h"

namespace llvm2Col {
    const std::string SOURCE_LOC = "Transform::Instruction::TermOp";


    void transformTermOp(llvm::Instruction &llvmInstruction,
                         col::Block &colBlock,
                         vcllvm::FunctionCursor &funcCursor) {
        switch (llvm::Instruction::TermOps(llvmInstruction.getOpcode())) {
            case llvm::Instruction::Ret:
                transformRet(cast<llvm::ReturnInst>(llvmInstruction), colBlock, funcCursor);
                break;
            case llvm::Instruction::Br: {
                auto &llvmBranchInst = cast<llvm::BranchInst>(llvmInstruction);
                llvmBranchInst.isConditional() ? transformConditionalBranch(llvmBranchInst, colBlock, funcCursor)
                                               : transformUnConditionalBranch(llvmBranchInst, colBlock, funcCursor);
                break;
            }
            default:
                reportUnsupportedOperatorError(SOURCE_LOC, llvmInstruction);
                break;
        }
    }

    void transformRet(llvm::ReturnInst &llvmRetInstruction,
                      col::Block &colBlock,
                      vcllvm::FunctionCursor &funcCursor) {
        col::Return *returnStatement = colBlock.add_statements()->mutable_return_();
        returnStatement->set_origin(generateSingleStatementOrigin(llvmRetInstruction));
        col::Expr *returnExpr = returnStatement->mutable_result();
        llvm2Col::transformAndSetExpr(
                funcCursor,
                llvmRetInstruction,
                *llvmRetInstruction.getReturnValue(),
                *returnExpr);
    }

    void transformConditionalBranch(llvm::BranchInst &llvmBrInstruction,
                                    col::Block &colBlock,
                                    vcllvm::FunctionCursor &funcCursor) {
        col::Branch *colBranch = colBlock.add_statements()->mutable_branch();

        // true branch
        col::ExprStatement *colTrueBranch = colBranch->add_branches();
        // set conditional
        transformAndSetExpr(funcCursor,
                            llvmBrInstruction,
                            *llvmBrInstruction.getCondition(),
                            *colTrueBranch->mutable_v1());
        // get or pre-generate target labeled block
        auto *llvmTrueBlock = cast<llvm::BasicBlock>(llvmBrInstruction.getOperand(1));
        vcllvm::LabeledColBlock labeledTrueColBlock = funcCursor.getOrSetLlvmBlock2LabeledColBlockEntry(*llvmTrueBlock);
        // goto statement to true block
        col::Goto *trueGoto = colTrueBranch->mutable_v2()->mutable_goto_();
        trueGoto->mutable_lbl()->set_index(labeledTrueColBlock.label.decl().id());
        // set origin for goto to true block
        trueGoto->set_origin(generateSingleStatementOrigin(llvmBrInstruction));
        // transform llvm true block
        transformLlvmBlock(*llvmTrueBlock, funcCursor);

        // false branch
        col::ExprStatement *colFalseBranch = colBranch->add_branches();
        // set conditional (which is a true constant as else == else if(true)))
        colFalseBranch->mutable_v1()->mutable_boolean_value()->set_value(true);
        // get llvm block targeted by the llvm branch
        auto *llvmFalseBlock = cast<llvm::BasicBlock>(llvmBrInstruction.getOperand(2));
        // get or pre-generate target labeled block
        vcllvm::LabeledColBlock labeledFalseColBlock = funcCursor.getOrSetLlvmBlock2LabeledColBlockEntry(
                *llvmFalseBlock);
        // goto statement to false block
        col::Goto *falseGoto = colFalseBranch->mutable_v2()->mutable_goto_();
        falseGoto->mutable_lbl()->set_index(labeledFalseColBlock.label.decl().id());
        // set origin for goto to false block
        falseGoto->set_origin(llvm2Col::generateSingleStatementOrigin(llvmBrInstruction));
        // transform llvm falseBlock
        transformLlvmBlock(*llvmFalseBlock, funcCursor);
    }

    void transformUnConditionalBranch(llvm::BranchInst &llvmBrInstruction,
                                      col::Block &colBlock,
                                      vcllvm::FunctionCursor &funcCursor) {
        // get llvm target block
        auto *llvmTargetBlock = cast<llvm::BasicBlock>(llvmBrInstruction.getOperand(0));
        // get or pre generate target labeled block
        vcllvm::LabeledColBlock labeledColBlock = funcCursor.getOrSetLlvmBlock2LabeledColBlockEntry(*llvmTargetBlock);
        // create goto to target labeled block
        col::Goto *colGoto = colBlock.add_statements()->mutable_goto_();
        colGoto->mutable_lbl()->set_index(labeledColBlock.label.decl().id());
        // set origin of goto statement
        colGoto->set_origin(llvm2Col::generateSingleStatementOrigin(llvmBrInstruction));
        // transform llvm targetBlock
        transformLlvmBlock(*llvmTargetBlock, funcCursor);
    }
}