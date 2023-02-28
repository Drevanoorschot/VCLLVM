#ifndef VCLLVM_FUNCTIONDECLARER_H
#define VCLLVM_FUNCTIONDECLARER_H

#include <llvm/IR/PassManager.h>

namespace llvm {
    class FunctionDeclarer : public PassInfoMixin<FunctionDeclarer> {

    };

}
#endif //VCLLVM_FUNCTIONDECLARER_H