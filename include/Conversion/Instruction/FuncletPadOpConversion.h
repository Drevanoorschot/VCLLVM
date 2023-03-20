#ifndef VCLLVM_FUNCLETPADOPCONVERSION_H
#define VCLLVM_FUNCLETPADOPCONVERSION_H

#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertFuncletPadOp(llvm::Instruction &llvmInstruction,
                             ColScopedBlock colScopedBlock,
                             vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_FUNCLETPADOPCONVERSION_H
