#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"
#include "Util/Conversion/Conversion.h"
#include "Util/Conversion/Instruction/UnaryOpConversion.h"
#include "Util/Conversion/Instruction/BinaryOpConversion.h"
#include "Util/Conversion/Instruction/MemoryOpConversion.h"
#include "Util/Conversion/Instruction/FuncletPadOpConversion.h"
#include "Util/Conversion/Instruction/OtherOpConversion.h"
#include "Util/Exceptions.h"

namespace col = vct::col::serialize;


/**
 * Utility function that converts LLVM types to col types
 * @param type
 */
namespace llvm2Col {
    void convertAndSetType(llvm::Type &llvmType, col::Type &colType) {
        switch (llvmType.getTypeID()) {
            case llvm::Type::IntegerTyID:
                if (llvmType.getIntegerBitWidth() == 1) {
                    throw vcllvm::UnsupportedTypeException("Boolean types not supported");
                }
                colType.mutable_t_int();
                break;
            default:
                throw vcllvm::UnsupportedTypeException("Type not supported");
        }
    }

    col::Block &setAndReturnScopedBlock(col::Statement &statement) {
        return *statement.mutable_scope()->mutable_body()->mutable_block();
    }


    void convertNonTermInstruction(llvm::Instruction &llvmInstruction,
                            col::Block &colBlock,
                            vcllvm::FunctionCursor &funcCursor) {
        if (llvmInstruction.isTerminator()) {
            vcllvm::ErrorReporter::addError("Util::Conversion", "Wrong method call to handle terminator instructions!");
            return;
        }
        if (convertUnaryOp(llvmInstruction, colBlock, funcCursor) ||
            convertBinaryOp(llvmInstruction, colBlock, funcCursor) ||
            convertMemoryOp(llvmInstruction, colBlock, funcCursor) ||
            convertFuncletPadOp(llvmInstruction, colBlock, funcCursor) ||
            convertOtherOp(llvmInstruction, colBlock, funcCursor)) {
            return;
        }
        std::stringstream errorStream;
        errorStream << "Unable to convert operator \"" << llvmInstruction.getOpcodeName() << "\" in function \""
                    << llvmInstruction.getFunction()->getName().str();
        vcllvm::ErrorReporter::addError("Util::Conversion", errorStream.str());
    }
}