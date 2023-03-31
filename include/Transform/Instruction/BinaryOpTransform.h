#ifndef VCLLVM_BINARYOPTRANSFORM_H
#define VCLLVM_BINARYOPTRANSFORM_H

#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionBodyTransformer.h"

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;


    void transformBinaryOp(llvm::Instruction &llvmInstruction,
                           col::Block &colBlock,
                           vcllvm::FunctionCursor &funcCursor);


    void transformOperands(auto &colBinExpr,
                           llvm::Instruction &llvmInstruction,
                           vcllvm::FunctionCursor &funcCursor);

    col::Plus &transformAdd(col::Assign &assignment);


    col::Minus &transformSub(col::Assign &assignment);

    col::Mult &transformMul(col::Assign &assignment);

    col::Div &transformDiv(col::Assign &assignment);
}
#endif //VCLLVM_BINARYOPTRANSFORM_H
