#include "Util/Conversion/Instruction/CastOpConversion.h"
#include "Util/Exceptions.h"

namespace llvm2Col {
    void convertCastOp(llvm::Instruction &llvmInstruction,
                       ColScopedBlock colScopedBlock,
                       vcllvm::FunctionCursor &funcCursor) {
        //TODO stub
        std::stringstream errorStream;
        errorStream << "Unsupported operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                    << llvmInstruction.getFunction()->getName().str();
        vcllvm::ErrorReporter::addError("Util::Conversion::Instruction::CastOp", errorStream.str());
    }
}