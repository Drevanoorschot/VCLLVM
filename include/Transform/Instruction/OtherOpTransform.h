#ifndef VCLLVM_OTHEROPTRANSFORM_H
#define VCLLVM_OTHEROPTRANSFORM_H
#include "Passes/Function/FunctionBodyTransformer.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void transformOtherOp(llvm::Instruction &llvmInstruction,
                          col::Block &colBlock,
                          vcllvm::FunctionCursor &funcCursor);
}

#endif //VCLLVM_OTHEROPTRANSFORM_H
