#ifndef VCLLVM_TRANSFORM_H
#define VCLLVM_TRANSFORM_H

#include <llvm/IR/Type.h>
#include "col.pb.h"
#include "Passes/Function/FunctionBodyTransformer.h"

/**
 * General helper functions for transformations
 */
namespace llvm2Col {
    namespace col = vct::col::serialize;

    // type transformers
    void transformAndSetType(llvm::Type &llvmType, col::Type &colType);

    void transformAndSetIntegerValue(col::IntegerValue &colIntegerValue, llvm::APInt &apInt);

    // expression transformers
    void transformAndSetExpr(llvm::Value &llvmValue, col::Expr &colExpr, vcllvm::FunctionCursor &functionCursor);

    void transformAndSetConstExpr(llvm::Constant &llvmConstant, col::Expr &colExpr);

    void transformAndSetVarExpr(llvm::Value &llvmValue, col::Expr &colExpr, vcllvm::FunctionCursor &functionCursor);

    template<class IDNode>
    void setColNodeId(IDNode &idNode) {
        idNode->set_id(reinterpret_cast<int64_t>(idNode));
    }

}
#endif //VCLLVM_TRANSFORM_H
