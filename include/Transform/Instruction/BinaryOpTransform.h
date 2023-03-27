#ifndef VCLLVM_BINARYOPTRANSFORM_H
#define VCLLVM_BINARYOPTRANSFORM_H

#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;


    void convertBinaryOp(llvm::Instruction &llvmInstruction,
                         col::Block &colBlock,
                         vcllvm::FunctionCursor &funcCursor);


    void convertOperands(auto &binExpr,
                         llvm::Instruction &llvmInstruction,
                         vcllvm::FunctionCursor &funcCursor);

    col::Plus &convertAdd(col::Assign &assignment);


    col::Minus &convertSub(col::Assign &assignment);

    col::Mult &convertMul(col::Assign &assignment);

    col::Div &convertDiv(col::Assign &assignment);

//    template<class BinExpr> void convertOperands()
}
#endif //VCLLVM_BINARYOPTRANSFORM_H
