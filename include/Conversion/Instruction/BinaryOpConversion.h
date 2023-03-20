#ifndef VCLLVM_BINARYOPCONVERSION_H
#define VCLLVM_BINARYOPCONVERSION_H

#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;


    void convertBinaryOp(llvm::Instruction &llvmInstruction,
                         ColScopedBlock colScopedBlock,
                         vcllvm::FunctionCursor &funcCursor);


    col::Plus &convertAdd(col::Assign &assignment);


    void convertOperands(auto &binExpr,
                         llvm::Instruction &llvmInstruction,
                         ColScopedBlock colScopedBlock,
                         vcllvm::FunctionCursor &funcCursor);

//    template<class BinExpr> void convertOperands()
}
#endif //VCLLVM_BINARYOPCONVERSION_H
