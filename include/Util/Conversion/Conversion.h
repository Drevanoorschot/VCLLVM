#ifndef VCLLVM_CONVERSION_H
#define VCLLVM_CONVERSION_H

#include <llvm/IR/Type.h>
#include "Passes/Function/FunctionInstructionTransformer.h"
#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertAndSetType(llvm::Type &llvmType, col::Type &colType);

    template<class IDNode>

    void setColNodeId(IDNode &idNode) {
        idNode->set_id(reinterpret_cast<int64_t>(idNode));
    }

    col::Block &setAndReturnScopedBlock(col::Statement &statement);

    /**
    * converts non terminating instructions (i.e. everything that guarantees to fit into a COL-block)
    */
    void convertNonTermInstruction(llvm::Instruction &llvmInstruction,
                                   col::Block &colBlock,
                                   vcllvm::FunctionCursor &funcCursor);

}
#endif //VCLLVM_CONVERSION_H
