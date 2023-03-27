#include "Conversion/Instruction/FuncletPadOpConversion.h"
#include "Util/Exceptions.h"

namespace llvm2Col {
    void convertFuncletPadOp(llvm::Instruction &llvmInstruction,
                             col::Block &colBlock,
                             vcllvm::FunctionCursor &funcCursor) {
        //TODO stub
        std::stringstream errorStream;
        errorStream << "Unsupported operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                    << llvmInstruction.getFunction()->getName().str();
        vcllvm::ErrorReporter::addError("Util::Conversion::Instruction::FuncletPadOp", errorStream.str());
    }
}