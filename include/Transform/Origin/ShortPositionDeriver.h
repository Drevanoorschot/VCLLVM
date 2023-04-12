#ifndef VCLLVM_SHORTPOSITIONDERIVER_H
#define VCLLVM_SHORTPOSITIONDERIVER_H

#include <llvm/IR/Module.h>

namespace llvm2Col {
    std::string deriveModuleShortPosition(llvm::Module &llvmModule);

    std::string deriveFunctionShortPosition(llvm::Function &llvmFunction);

    std::string deriveBlockShortPosition(llvm::BasicBlock &llvmBlock);

    std::string deriveInstructionShortPosition(llvm::Instruction &llvmInstruction);
}
#endif //VCLLVM_SHORTPOSITIONDERIVER_H
