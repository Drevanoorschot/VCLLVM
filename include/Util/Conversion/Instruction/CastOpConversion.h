#ifndef VCLLVM_CASTOPCONVERSION_H
#define VCLLVM_CASTOPCONVERSION_H
#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"
namespace llvm2Col {
    namespace col = vct::col::serialize;

    bool convertCastOp(llvm::Instruction &llvmInstruction,
                        col::Block &colBlock,
                        vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_CASTOPCONVERSION_H
