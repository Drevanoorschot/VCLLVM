#ifndef VCLLVM_BINARYOPCONVERSION_H
#define VCLLVM_BINARYOPCONVERSION_H
#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"
namespace llvm2Col {
    namespace col = vct::col::serialize;

    bool convertBinaryOp(llvm::Instruction &llvmInstruction,
                         ColScopedBlock colScopedBlock,
                         vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_BINARYOPCONVERSION_H
