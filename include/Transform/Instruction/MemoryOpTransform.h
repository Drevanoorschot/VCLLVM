#ifndef VCLLVM_MEMORYOPTRANSFORM_H
#define VCLLVM_MEMORYOPTRANSFORM_H
#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionBodyTransformer.h"

#include "col.pb.h"
namespace llvm2Col {
    namespace col = vct::col::serialize;

    void transformMemoryOp(llvm::Instruction &llvmInstruction,
                           col::Block &colBlock,
                           vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_MEMORYOPTRANSFORM_H
