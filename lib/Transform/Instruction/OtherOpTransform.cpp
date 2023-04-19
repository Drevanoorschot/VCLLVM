#include "Transform/Instruction/OtherOpTransform.h"

#include "Transform/BlockTransform.h"
#include "Util/Exceptions.h"

namespace llvm2Col {
    const std::string SOURCE_LOC = "Transform::Instruction::OtherOp";

    void transformOtherOp(llvm::Instruction &llvmInstruction,
                          col::Block &colBlock,
                          vcllvm::FunctionCursor &funcCursor) {
        //TODO stub
        reportUnsupportedOperatorError(SOURCE_LOC, llvmInstruction);
    }
}