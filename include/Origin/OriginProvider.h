#ifndef VCLLVM_ORIGINPROVIDER_H
#define VCLLVM_ORIGINPROVIDER_H

#include <llvm/IR/Value.h>

/**
 * Generators for VerCors origin objects for various LLVM Value types.
 *
 * For more info on VerCors origins see: https://github.com/utwente-fmt/vercors/discussions/884
 */
namespace llvm2Col {

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



