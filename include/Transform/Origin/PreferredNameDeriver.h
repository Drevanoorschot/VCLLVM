#ifndef VCLLVM_PREFERREDNAMEDERIVER_H
#define VCLLVM_PREFERREDNAMEDERIVER_H

#include <llvm/IR/Value.h>

namespace llvm2Col {
    std::string deriveOperandPreferredName(llvm::Value &llvmOperand);
}
#endif //VCLLVM_PREFERREDNAMEDERIVER_H
