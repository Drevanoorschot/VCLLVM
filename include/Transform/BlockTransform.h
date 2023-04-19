#ifndef VCLLVM_BLOCKTRANSFORM_H
#define VCLLVM_BLOCKTRANSFORM_H
#include "Passes/Function/FunctionBodyTransformer.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    /**
     * Entry point for each block transformation. It performs the following steps:
     * <ol>
     * <li>Create or fetch the corresponding labeled col block from the function cursor</li>
     * <li>Check if all predecessor blocks have been visited yet, otherwise, return</li>
     * <li>If block turns out to be a loop header, hand over control to the <code>transformLoop</code> function.
     * Else, transform instructions of the block</li>
     * </ol>
     *
     * Note: The <code>transformTermOp</code> function will take care of subsequent blocks recursively
     * @param functionCursor
     * @param llvmBlock
     */
    void transformLlvmBlock(llvm::BasicBlock &llvmBlock, vcllvm::FunctionCursor &functionCursor);

    void transformLoop(llvm::BasicBlock &llvmBlock, vcllvm::FunctionCursor &functionCursor);

    void transformInstruction(vcllvm::FunctionCursor &funcCursor,
                              llvm::Instruction &llvmInstruction,
                              col::Block &colBodyBlock);

    void reportUnsupportedOperatorError(const std::string &source, llvm::Instruction &llvmInstruction);
}
#endif //VCLLVM_BLOCKTRANSFORM_H

