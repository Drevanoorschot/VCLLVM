#ifndef VCLLVM_OTHEROPCONVERSION_H
#define VCLLVM_OTHEROPCONVERSION_H

#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertOtherOp(llvm::Instruction &llvmInstruction,
                        ColScopedBlock colScopedBlock,
                        vcllvm::FunctionCursor &funcCursor);
}

#endif //VCLLVM_OTHEROPCONVERSION_H
