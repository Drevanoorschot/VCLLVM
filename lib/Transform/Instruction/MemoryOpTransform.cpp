#include "Transform/Instruction/MemoryOpTransform.h"
#include "Util/Exceptions.h"

namespace llvm2Col {
    void convertMemoryOp(llvm::Instruction &llvmInstruction,
                         col::Block &colBlock,
                         vcllvm::FunctionCursor &funcCursor) {
        //TODO stub
        std::stringstream errorStream;
        errorStream << "Unsupported operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                    << llvmInstruction.getFunction()->getName().str();
        vcllvm::ErrorReporter::addError("Util::Transform::Instruction::MemoryOp", errorStream.str());
    }
}