#include "Origin/OriginProvider.h"

#include <llvm/IR/Module.h>

#include "nlohmann/json.hpp"

#include "Origin/PreferredNameDeriver.h"
#include "Origin/ContextDeriver.h"
#include "Origin/ShortPositionDeriver.h"

namespace llvm2Col {
    using json = nlohmann::json;

    const std::string PREFERRED_NAME = "preferredName";
    const std::string CONTEXT = "context";
    const std::string INLINE_CONTEXT = "inlineContext";
    const std::string SHORT_POSITION = "shortPosition";

    const std::string EMPTY_ORIGIN = json("{}").dump();

    std::string generateProgramOrigin(llvm::Module &llvmModule) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, "program:" + llvmModule.getName().str()});
        originMap.insert({CONTEXT, "At: " + deriveModuleContext(llvmModule)});
        originMap.insert({INLINE_CONTEXT, deriveModuleContext(llvmModule)});
        originMap.insert({SHORT_POSITION, deriveModuleShortPosition(llvmModule)});

        return json(originMap).dump();
    }

    std::string generateFuncDefOrigin(llvm::Function &llvmFunction) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, llvmFunction.getName().str()});
        originMap.insert({CONTEXT, "At " + deriveFunctionContext(llvmFunction)});
        originMap.insert({INLINE_CONTEXT, deriveFunctionContext(llvmFunction)});
        originMap.insert({SHORT_POSITION, deriveFunctionShortPosition(llvmFunction)});

        return json(originMap).dump();
    }

    std::string generateFunctionContractOrigin(llvm::MDNode &contractMDNode) {
        // assumption: it's going to get parsed by vercors again which will fix the origins.
        return EMPTY_ORIGIN;
    }

    std::string generateArgumentOrigin(llvm::Argument &llvmArgument) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, deriveArgumentPreferredName(llvmArgument)});
        originMap.insert({CONTEXT, "At: " + deriveFunctionContext(*llvmArgument.getParent())});
        originMap.insert({INLINE_CONTEXT, deriveFunctionContext(*llvmArgument.getParent())});
        originMap.insert({SHORT_POSITION, deriveFunctionShortPosition(*llvmArgument.getParent())});

        return json(originMap).dump();
    }

    std::string generateBlockOrigin(llvm::BasicBlock &llvmBlock) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, "block"});
        originMap.insert({CONTEXT, "At: " + deriveBlockContext(llvmBlock)});
        originMap.insert({INLINE_CONTEXT, deriveBlockContext(llvmBlock)});
        originMap.insert({SHORT_POSITION, deriveBlockShortPosition(llvmBlock)});

        return json(originMap).dump();
    }

    std::string generateLabelOrigin(llvm::BasicBlock &llvmBlock) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, "label"});
        originMap.insert({CONTEXT, "At: " + deriveLabelContext(llvmBlock)});
        originMap.insert({INLINE_CONTEXT, deriveLabelContext(llvmBlock)});
        originMap.insert({SHORT_POSITION, deriveBlockShortPosition(llvmBlock)});

        return json(originMap).dump();
    }

    std::string generateSingleStatementOrigin(llvm::Instruction &llvmInstruction) {
        std::unordered_map<std::string, std::string> originMap;

        // omit preferred name
        originMap.insert({CONTEXT, "At: " + deriveSurroundingInstructionContext(llvmInstruction)});
        originMap.insert({INLINE_CONTEXT, deriveInstructionContext(llvmInstruction)});
        originMap.insert({SHORT_POSITION, deriveInstructionShortPosition(llvmInstruction)});

        return json(originMap).dump();
    }

    std::string generateAssignTargetOrigin(llvm::Instruction &llvmInstruction) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, "var"});
        originMap.insert({CONTEXT, "At: " + deriveInstructionContext(llvmInstruction)});
        originMap.insert({INLINE_CONTEXT, deriveInstructionLhs(llvmInstruction)});
        originMap.insert({SHORT_POSITION, deriveInstructionShortPosition(llvmInstruction)});

        return json(originMap).dump();
    }

    std::string generateBinExprOrigin(llvm::Instruction &llvmInstruction) {
        std::unordered_map<std::string, std::string> originMap;

        // omit preferred name
        originMap.insert({CONTEXT, "At " + deriveInstructionContext(llvmInstruction)});
        originMap.insert({INLINE_CONTEXT, deriveInstructionRhs(llvmInstruction)});
        originMap.insert({SHORT_POSITION, deriveInstructionShortPosition(llvmInstruction)});

        return json(originMap).dump();
    }

    std::string generateOperandOrigin(llvm::Instruction &llvmInstruction, llvm::Value &llvmOperand) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, deriveOperandPreferredName(llvmOperand)});
        originMap.insert({CONTEXT, "At " + deriveInstructionContext(llvmInstruction)});
        originMap.insert({INLINE_CONTEXT, deriveOperandContext(llvmOperand)});
        originMap.insert({SHORT_POSITION, deriveInstructionShortPosition(llvmInstruction)});

        return json(originMap).dump();
    }

    std::string generateTypeOrigin(llvm::Type &llvmType) {
        std::unordered_map<std::string, std::string> originMap;

        originMap.insert({PREFERRED_NAME, deriveTypePreferredName(llvmType)});

        return json(originMap).dump();
    }


}
