#ifndef VCLLVM_BLOCKTRANSFORM_H
#define VCLLVM_BLOCKTRANSFORM_H


#include <llvm/IR/BasicBlock.h>

#include "Passes/Function/FunctionBodyTransformer.h"

#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void transformLlvmBlock(vcllvm::FunctionCursor &functionCursor,
                            col::Block &currentColBody,
                            llvm::BasicBlock &llvmBlock);

    void transformConditionalBranch(vcllvm::FunctionCursor &functionCursor,
                                    col::Block &currentColBody,
                                    llvm::BasicBlock &falseLlvmBlock,
                                    llvm::BasicBlock &trueLlvmBlock);

    void transformUnconditionalBranch(vcllvm::FunctionCursor &functionCursor,
                                      col::Block &currentColBody,
                                      llvm::BasicBlock &llvmBlock);

    void transformLoop(vcllvm::FunctionCursor &functionCursor,
                       col::Block &currentColBody,
                       llvm::BasicBlock &llvmBlock);

    void convertNonTermInstruction(llvm::Instruction &llvmInstruction,
                                   col::Block &colBlock,
                                   vcllvm::FunctionCursor &funcCursor);
}
#endif //VCLLVM_BLOCKTRANSFORM_H

