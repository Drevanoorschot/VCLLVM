#ifndef VCLLVM_TRANSFORM_H
#define VCLLVM_TRANSFORM_H

#include "Passes/Function/FunctionBodyTransformer.h"
#include "Origin/OriginProvider.h"

/**
 * General helper functions for transformations
 */

namespace llvm2Col {
    namespace col = vct::col::serialize;

    // type transformers
    void transformAndSetType(llvm::Type &llvmType, col::Type &colType);

    void transformAndSetIntegerValue(llvm::APInt &apInt, col::IntegerValue &colIntegerValue);

    // expression transformers
    void transformAndSetExpr(vcllvm::FunctionCursor &functionCursor, llvm::Instruction &llvmInstruction,
                             llvm::Value &llvmOperand, col::Expr &colExpr);

    void transformAndSetConstExpr(llvm::Instruction &llvmInstruction, llvm::Constant &llvmConstant, col::Expr &colExpr);

    void transformAndSetVarExpr(vcllvm::FunctionCursor &functionCursor, llvm::Instruction &llvmInstruction,
                                llvm::Value &llvmOperand, col::Expr &colExpr);
    template<class ColBinExpr>
    void transformBinExpr(llvm::Instruction &llvmInstruction,
                          ColBinExpr &colBinExpr,
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

    template<class IDNode>
    void setColNodeId(IDNode &idNode) {
        idNode->set_id(reinterpret_cast<int64_t>(idNode));
    }

    std::string getValueName(llvm::Value &llvmValue);
}
#endif //VCLLVM_TRANSFORM_H
