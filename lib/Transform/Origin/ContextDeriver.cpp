#include "Transform/Origin/ContextDeriver.h"

#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

namespace llvm2Col {
    // module derivers
    std::string deriveModuleContext(llvm::Module &llvmModule) {
        std::string context;
        llvm::raw_string_ostream(context) << llvmModule;
        return context;
    }

    // function derivers
    std::string deriveFunctionContext(llvm::Function &llvmFunction) {
        std::string context;
        llvm::raw_string_ostream(context) << llvmFunction;
        return context;
    }

    // block derivers

    // instruction derivers

    std::string deriveSurroundingInstructionContext(llvm::Instruction &llvmInstruction) {
        std::string context;
        if (llvmInstruction.getPrevNode() != nullptr) {
            llvm::raw_string_ostream(context) << llvmInstruction.getPrevNode() << '\n';
        }
        llvm::raw_string_ostream(context) << llvmInstruction;
        if (llvmInstruction.getNextNode() != nullptr) {
            llvm::raw_string_ostream(context) << '\n' << llvmInstruction.getNextNode();
        }
        return context;
    }

    std::string deriveInstructionContext(llvm::Instruction &llvmInstruction) {
        std::string context;
        llvm::raw_string_ostream(context) << llvmInstruction;
        return context;
    }

    std::string deriveInstructionRhs(llvm::Instruction &llvmInstruction) {
        std::string fullContext = deriveInstructionContext(llvmInstruction);
        return fullContext.substr(fullContext.find('='));
    }

    std::string deriveOperandContext(llvm::Value &llvmOperand) {
        std::string context;
        llvm::raw_string_ostream contextStream = llvm::raw_string_ostream(context);
        llvmOperand.printAsOperand(contextStream, false);
        return context;
    }
}