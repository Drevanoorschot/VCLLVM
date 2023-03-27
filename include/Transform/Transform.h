#ifndef VCLLVM_TRANSFORM_H
#define VCLLVM_TRANSFORM_H

#include <llvm/IR/Type.h>
#include "col.pb.h"

namespace llvm2Col {
    namespace col = vct::col::serialize;

    void convertAndSetType(llvm::Type &llvmType, col::Type &colType);

    template<class IDNode>

    void setColNodeId(IDNode &idNode) {
        idNode->set_id(reinterpret_cast<int64_t>(idNode));
    }

}
#endif //VCLLVM_TRANSFORM_H
