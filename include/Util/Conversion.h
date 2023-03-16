#ifndef VCLLVM_CONVERSION_H
#define VCLLVM_CONVERSION_H

#include <llvm/IR/Type.h>
#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertAndSetType(llvm::Type &llvmType, col::Type &colType);

    template<class IDNode>

    void setColNodeId(IDNode &idNode) {
        idNode->set_id(reinterpret_cast<int64_t>(idNode));
    }

    col::Block &setAndReturnScopedBlock(col::Statement &statement);

}
#endif //VCLLVM_CONVERSION_H
