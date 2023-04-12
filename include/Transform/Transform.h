#ifndef VCLLVM_TRANSFORM_H
#define VCLLVM_TRANSFORM_H

#include "Passes/Function/FunctionBodyTransformer.h"

/**
 * General helper functions for transformations
 */

namespace llvm2Col {
    namespace col = vct::col::serialize;

    // type transformers
    void transformAndSetType(llvm::Type &llvmType, col::Type &colType);

    void transformAndSetIntegerValue(llvm::APInt &apInt, col::IntegerValue &colIntegerValue);

    // expression transformers
    void transformAndSetExpr(vcllvm::FunctionCursor &functionCursor, llvm::Value &llvmValue, col::Expr &colExpr);

    void transformAndSetConstExpr(llvm::Constant &llvmConstant, col::Expr &colExpr);

    void transformAndSetVarExpr(vcllvm::FunctionCursor &functionCursor, llvm::Value &llvmValue, col::Expr &colExpr);

    template<class IDNode>
    void setColNodeId(IDNode &idNode) {
        idNode->set_id(reinterpret_cast<int64_t>(idNode));
    }

}
#endif //VCLLVM_TRANSFORM_H
