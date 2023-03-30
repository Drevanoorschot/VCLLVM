#ifndef VCLLVM_OTHEROPTRANSFORM_H
#define VCLLVM_OTHEROPTRANSFORM_H

#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionBodyTransformer.h"

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertOtherOp(llvm::Instruction &llvmInstruction,
                        col::Block &colBlock,
                        vcllvm::FunctionCursor &funcCursor);
}

#endif //VCLLVM_OTHEROPTRANSFORM_H
