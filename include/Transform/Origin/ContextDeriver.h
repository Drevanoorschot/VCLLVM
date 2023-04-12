#ifndef VCLLVM_CONTEXTDERIVER_H
#define VCLLVM_CONTEXTDERIVER_H

#include <llvm/IR/Value.h>

namespace llvm2Col {
    // function derivers
    std::string deriveFunctionContext(llvm::Function &llvmFunction);

    // block derivers

    // instruction derivers
    std::string deriveSurroundingInstructionContext(llvm::Instruction &llvmInstruction);

    std::string deriveInstructionContext(llvm::Instruction &llvmInstruction);

    std::string deriveInstructionRhs(llvm::Instruction &llvmInstruction);

    // operand derivers
    std::string deriveOperandContext(llvm::Value &llvmOperand);
}
#endif //VCLLVM_CONTEXTDERIVER_H
