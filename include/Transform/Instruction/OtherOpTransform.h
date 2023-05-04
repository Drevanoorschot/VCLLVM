#ifndef VCLLVM_OTHEROPTRANSFORM_H
#define VCLLVM_OTHEROPTRANSFORM_H
#include "Passes/Function/FunctionBodyTransformer.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void transformOtherOp(llvm::Instruction &llvmInstruction,
                          col::Block &colBlock,
                          vcllvm::FunctionCursor &funcCursor);

    void transformPhi(llvm::PHINode &phiInstruction, vcllvm::FunctionCursor &funcCursor);

    void transformICmp(llvm::ICmpInst &icmpInstruction,
                       col::Block &colBlock,
                       vcllvm::FunctionCursor &funcCursor);

    void transformCmpExpr(llvm::CmpInst &cmpInstruction,
                          auto &colCompareExpr,
                          vcllvm::FunctionCursor &funcCursor);
}

#endif //VCLLVM_OTHEROPTRANSFORM_H
