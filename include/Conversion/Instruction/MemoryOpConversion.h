#ifndef VCLLVM_MEMORYOPCONVERSION_H
#define VCLLVM_MEMORYOPCONVERSION_H
#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"
namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertMemoryOp(llvm::Instruction &llvmInstruction,
                         ColScopedBlock colScopedBlock,
                         vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_MEMORYOPCONVERSION_H
