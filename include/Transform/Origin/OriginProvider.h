#ifndef VCLLVM_ORIGINPROVIDER_H
#define VCLLVM_ORIGINPROVIDER_H

#include <llvm/IR/Value.h>

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    std::string generateProgramOrigin(llvm::Module &llvmModule);

    std::string generateFuncDefOrigin(llvm::Function &llvmFunction);

    std::string generateFunctionContractOrigin(llvm::MDNode &contractMDNode);

    std::string generateArgumentOrigin(llvm::Argument &llvmArgument);

    std::string generateBlockOrigin(llvm::BasicBlock &llvmBlock);

    std::string generateLabelOrigin(llvm::BasicBlock &llvmBlock);

    std::string generateSingleStatementOrigin(llvm::Instruction &llvmInstruction);

    std::string generateAssignTargetOrigin(llvm::Instruction &llvmInstruction);

    std::string generateBinExprOrigin(llvm::Instruction &llvmInstruction);

    std::string generateOperandOrigin(llvm::Instruction &llvmInstruction, llvm::Value &llvmOperand);

    std::string generateTypeOrigin(llvm::Type &llvmType);


}
#endif //VCLLVM_ORIGINPROVIDER_H



