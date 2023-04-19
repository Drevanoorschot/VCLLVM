#ifndef VCLLVM_BINARYOPTRANSFORM_H
#define VCLLVM_BINARYOPTRANSFORM_H

#include "Passes/Function/FunctionBodyTransformer.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;


    void transformBinaryOp(llvm::Instruction &llvmInstruction,
                           col::Block &colBlock,
                           vcllvm::FunctionCursor &funcCursor);


    void transformBinExpr(llvm::Instruction &llvmInstruction,
                          auto &colBinExpr,
                          vcllvm::FunctionCursor &funcCursor);

}
#endif //VCLLVM_BINARYOPTRANSFORM_H
