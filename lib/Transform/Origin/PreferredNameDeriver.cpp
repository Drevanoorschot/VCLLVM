#include <llvm/IR/Constant.h>
#include <llvm/Support/raw_ostream.h>
#include "Transform/Origin/PreferredNameDeriver.h"

namespace llvm2Col {
    std::string deriveOperandPreferredName(llvm::Value &llvmOperand) {
        std::string preferredName;
        llvm::raw_string_ostream preferredNameStream = llvm::raw_string_ostream(preferredName);
        preferredNameStream << (llvm::isa<llvm::Constant>(llvmOperand) ? "const_" : "var_");
        llvmOperand.printAsOperand(preferredNameStream, false);
        return preferredName;
    }
}