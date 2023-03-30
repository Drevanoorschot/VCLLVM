#ifndef VCLLVM_CASTOPTRANSFORM_H
#define VCLLVM_CASTOPTRANSFORM_H
#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionBodyTransformer.h"

#include "col.pb.h"
namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertCastOp(llvm::Instruction &llvmInstruction,
                       col::Block &colBlock,
                       vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_CASTOPTRANSFORM_H
