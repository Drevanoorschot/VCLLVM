#include "Transform/Instruction/TermOpTransform.h"
#include "Util/Exceptions.h"
#include "Transform/Transform.h"
#include "Transform/BlockTransform.h"

namespace llvm2Col {
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
                std::stringstream errorStream;
                errorStream << "Unsupported operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                            << llvmInstruction.getFunction()->getName().str();
                vcllvm::ErrorReporter::addError("Util::Transform::Instruction::TermOp", errorStream.str());
                break;
        }
    }

    void transformRet(llvm::ReturnInst &llvmRetInstruction,
                      col::Block &colBlock,
                      vcllvm::FunctionCursor &funcCursor) {
        col::Expr *returnExpr = colBlock.add_statements()
                ->mutable_return_()
                ->mutable_result();
        llvm2Col::transformAndSetExpr(
                funcCursor,
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
                            *llvmBrInstruction.getCondition(),
                            *colTrueBranch->mutable_v1());
        // get or pre-generate target labeled block
        auto *llvmTrueBlock = cast<llvm::BasicBlock>(llvmBrInstruction.getOperand(1));
        vcllvm::LabeledColBlock labeledTrueColBlock = funcCursor.getOrSetLlvmBlock2LabeledColBlockEntry(*llvmTrueBlock);
        // goto statement to True block
        colTrueBranch->mutable_v2()->mutable_goto_()
                ->mutable_lbl()
                ->set_index(labeledTrueColBlock.label.decl().id());
        // transform llvm trueBlock
        transformLlvmBlock(*llvmTrueBlock, funcCursor);

        // true branch
        col::ExprStatement *colFalseBranch = colBranch->add_branches();
        // set conditional (which is a true constant as else == else if(true)))
        colFalseBranch->mutable_v1()->mutable_boolean_value()->set_value(true);
        // get llvm block targeted by the llvm branch
        auto *llvmFalseBlock = cast<llvm::BasicBlock>(llvmBrInstruction.getOperand(2));
        // get or pre-generate target labeled block
        vcllvm::LabeledColBlock labeledFalseColBlock = funcCursor.getOrSetLlvmBlock2LabeledColBlockEntry(
                *llvmFalseBlock);
        // goto statement to False block
        colTrueBranch->mutable_v2()->mutable_goto_()
                ->mutable_lbl()
                ->set_index(labeledFalseColBlock.label.decl().id());
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
        colBlock.add_statements()->mutable_goto_()
                ->mutable_lbl()
                ->set_index(labeledColBlock.label.decl().id());
        // transform llvm targetBlock
        transformLlvmBlock(*llvmTargetBlock, funcCursor);
    }
}