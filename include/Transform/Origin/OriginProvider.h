#ifndef VCLLVM_ORIGINPROVIDER_H
#define VCLLVM_ORIGINPROVIDER_H

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    std::string generateFuncDefOrigin(llvm::Function &llvmFunction);

    std::string generateAssignmentOrigin(llvm::Instruction &llvmInstruction);

    std::string generateBinExprOrigin(llvm::Instruction &llvmInstruction);

    std::string generateOperandOrigin(llvm::Instruction &llvmInstruction, llvm::Value &llvmOperand);
}
#endif //VCLLVM_ORIGINPROVIDER_H



