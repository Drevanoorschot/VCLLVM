#ifndef VCLLVM_BINARYOPTRANSFORM_H
#define VCLLVM_BINARYOPTRANSFORM_H

#include "Passes/Function/FunctionBodyTransformer.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;


    void transformBinaryOp(llvm::Instruction &llvmInstruction,
                           col::Block &colBlock,
                           vcllvm::FunctionCursor &funcCursor);


    void transformOperands(llvm::Instruction &llvmInstruction,
                           auto &colBinExpr,
                           vcllvm::FunctionCursor &funcCursor);

    col::Plus &transformAdd(col::Assign &assignment);

    col::Minus &transformSub(col::Assign &assignment);

    col::Mult &transformMul(col::Assign &assignment);

    col::FloorDiv &transformDiv(col::Assign &assignment);
}
#endif //VCLLVM_BINARYOPTRANSFORM_H
