#include "Transform/Origin/OriginProvider.h"

#include <llvm/IR/Module.h>

#include "nlohmann/json.hpp"

#include "Transform/Origin/PreferredNameDeriver.h"
#include "Transform/Origin/ContextDeriver.h"
#include "Transform/Origin/ShortPositionDeriver.h"

namespace llvm2Col {
    using json = nlohmann::json;

    const std::string PREFERRED_NAME = "preferredName";
    const std::string CONTEXT = "context";
    const std::string INLINE_CONTEXT = "inlineContext";
    const std::string SHORT_POSITION = "shortPosition";

    std::string generateFuncDefOrigin(llvm::Function &llvmFunction) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, llvmFunction.getName().str()});
        originMap.insert({CONTEXT, "At " + deriveFunctionContext(llvmFunction)});
        originMap.insert({INLINE_CONTEXT, deriveFunctionContext(llvmFunction)});
        originMap.insert({SHORT_POSITION, deriveFunctionShortPosition(llvmFunction)});

        return json(originMap).dump();
    }

    std::string generateAssignmentOrigin(llvm::Instruction &llvmInstruction) {
        std::unordered_map<std::string, std::string> originMap;

        // omit preferred name
        originMap.insert({CONTEXT, "At: " + deriveSurroundingInstructionContext(llvmInstruction)});
        originMap.insert({INLINE_CONTEXT, deriveInstructionContext(llvmInstruction)});
        originMap.insert({SHORT_POSITION, deriveInstructionShortPosition(llvmInstruction)});

        return json(originMap).dump();
    }

    std::string generateBinExprOrigin(llvm::Instruction &llvmInstruction) {
        std::unordered_map<std::string, std::string> originMap;

        // omit preferred name
        originMap.insert({CONTEXT, "At" + deriveInstructionContext(llvmInstruction)});
        originMap.insert({INLINE_CONTEXT, deriveInstructionRhs(llvmInstruction)});
        originMap.insert({SHORT_POSITION, deriveInstructionShortPosition(llvmInstruction)});

        return json(originMap).dump();
    }

    std::string generateOperandOrigin(llvm::Instruction &llvmInstruction, llvm::Value &llvmOperand) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, deriveOperandPreferredName(llvmOperand)});
        originMap.insert({CONTEXT, deriveInstructionContext(llvmInstruction)});
        originMap.insert({INLINE_CONTEXT, deriveOperandContext(llvmOperand)});
        originMap.insert({SHORT_POSITION, deriveInstructionShortPosition(llvmInstruction)});

        return json(originMap).dump();
        // TODO insert method at relevant places (binaryoptransformer + termoptransformer + func arguments + ...)
        // TODO test with vercors
        // TODO other origins: block, function missing parts
    }
}
