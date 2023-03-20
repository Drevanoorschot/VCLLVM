#ifndef VCLLVM_UNARYOPCONVERSION_H
#define VCLLVM_UNARYOPCONVERSION_H


#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"
namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertUnaryOp(llvm::Instruction &llvmInstruction,
                        ColScopedBlock colScopedBlock,
                        vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_UNARYOPCONVERSION_H