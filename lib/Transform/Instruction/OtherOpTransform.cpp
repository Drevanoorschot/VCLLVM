#include <llvm/IR/FMF.h>
#include "Transform/Instruction/OtherOpTransform.h"

#include "Transform/BlockTransform.h"
#include "Transform/Transform.h"
#include "Util/Exceptions.h"

namespace llvm2Col {
    const std::string SOURCE_LOC = "Transform::Instruction::OtherOp";

    void transformOtherOp(llvm::Instruction &llvmInstruction,
                          col::Block &colBlock,
                          vcllvm::FunctionCursor &funcCursor) {
        switch (llvm::Instruction::OtherOps(llvmInstruction.getOpcode())) {
            case llvm::Instruction::PHI:
                transformPhi(llvm::cast<llvm::PHINode>(llvmInstruction), funcCursor);
                break;
            case llvm::Instruction::ICmp:
                transformICmp(llvm::cast<llvm::ICmpInst>(llvmInstruction), colBlock, funcCursor);
                break;
            case llvm::Instruction::Call:
                transformCallExpr(llvm::cast<llvm::CallInst>(llvmInstruction), colBlock, funcCursor);
                break;
            default:
                reportUnsupportedOperatorError(SOURCE_LOC, llvmInstruction);
        }
    }

    void transformPhi(llvm::PHINode &phiInstruction,
                      vcllvm::FunctionCursor &funcCursor) {
        col::Variable &varDecl = funcCursor.declareVariable(phiInstruction);
        for (auto &B: phiInstruction.blocks()) {
            // add assignment of the variable to target block
            col::Block &targetBlock = funcCursor.getOrSetLlvmBlock2LabeledColBlockEntry(*B).block;
            col::Assign &assignment = funcCursor.createAssignment(phiInstruction, targetBlock, varDecl);
            // assign correct value by looking at the value-block pair of phi instruction.
            col::Expr *value = assignment.mutable_value();
            llvm2Col::transformAndSetExpr(funcCursor, phiInstruction,
                                          *phiInstruction.getIncomingValueForBlock(B), *value);
        }
    }

    void transformICmp(llvm::ICmpInst &icmpInstruction,
                       col::Block &colBlock,
                       vcllvm::FunctionCursor &funcCursor) {
        // we only support integer comparison
        if (not icmpInstruction.getOperand(0)->getType()->isIntegerTy()) {
            vcllvm::ErrorReporter::addError(SOURCE_LOC, "Unsupported compare type", icmpInstruction);
            return;
        }
        col::Assign &assignment = funcCursor.createAssignmentAndDeclaration(icmpInstruction, colBlock);
        switch (llvm::ICmpInst::Predicate(icmpInstruction.getPredicate())) {
            case llvm::CmpInst::ICMP_EQ: {
                col::Eq &eq = *assignment.mutable_value()->mutable_eq();
                transformCmpExpr(icmpInstruction, eq, funcCursor);
                break;
            }
            case llvm::CmpInst::ICMP_NE: {
                col::Neq &neq = *assignment.mutable_value()->mutable_neq();
                transformCmpExpr(icmpInstruction, neq, funcCursor);
                break;
            }
            case llvm::CmpInst::ICMP_SGT:
            case llvm::CmpInst::ICMP_UGT: {
                col::Greater &gt = *assignment.mutable_value()->mutable_greater();
                transformCmpExpr(icmpInstruction, gt, funcCursor);
                break;
            }
            case llvm::CmpInst::ICMP_SGE:
            case llvm::CmpInst::ICMP_UGE: {
                col::GreaterEq &geq = *assignment.mutable_value()->mutable_greater_eq();
                transformCmpExpr(icmpInstruction, geq, funcCursor);
                break;
            }
            case llvm::CmpInst::ICMP_SLT:
            case llvm::CmpInst::ICMP_ULT: {
                col::Less &lt = *assignment.mutable_value()->mutable_less();
                transformCmpExpr(icmpInstruction, lt, funcCursor);
                break;
            }
            case llvm::CmpInst::ICMP_SLE:
            case llvm::CmpInst::ICMP_ULE: {
                col::LessEq &leq = *assignment.mutable_value()->mutable_less_eq();
                transformCmpExpr(icmpInstruction, leq, funcCursor);
                break;
            }
            default:
                vcllvm::ErrorReporter::addError(SOURCE_LOC, "Unknown ICMP predicate", icmpInstruction);
        }
    }

    void transformCmpExpr(llvm::CmpInst &cmpInstruction,
                          auto &colCompareExpr,
                          vcllvm::FunctionCursor &funcCursor) {
        transformBinExpr(cmpInstruction, colCompareExpr, funcCursor);
    }

    void checkCallSupport(llvm::CallInst &callInstruction) {
        // tail recursion
        if (callInstruction.isMustTailCall() || callInstruction.isNoTailCall()) {
            vcllvm::ErrorReporter::addError(SOURCE_LOC, "Tail call optimization not supported", callInstruction);
        }
        // fast math
        if (callInstruction.getFastMathFlags().any()) {
            vcllvm::ErrorReporter::addError(SOURCE_LOC, "Fast math not supported", callInstruction);
        }
        // return attributes
        for(auto &A: callInstruction.getAttributes().getRetAttrs()) {
            std::stringstream errorStream;
            errorStream << "Return attribute" << A.getKindAsString().str() << "\" not supported";
            vcllvm::ErrorReporter::addError(SOURCE_LOC, errorStream.str(), callInstruction);
        }
        // address space is platform dependent (unlikely to change semantics)
        // function attributes are just extra compiler information (no semanatic changes)

        // operand bundles
        if (callInstruction.hasOperandBundles()) {
            vcllvm::ErrorReporter::addError(SOURCE_LOC, "Operand bundles not supported", callInstruction);
        }
    }

    void transformCallExpr(llvm::CallInst &callInstruction,
                           col::Block &colBlock,
                           vcllvm::FunctionCursor &funcCursor) {
        checkCallSupport(callInstruction);

    }
}