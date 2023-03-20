#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instruction.h>

#include "Passes/Function/FunctionInstructionTransformer.h"
#include "Conversion/Conversion.h"
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

    ColScopedBlock setAndReturnScopedBlock(col::Statement &statement) {
        ColScopedBlock colScopedBlock{};
        colScopedBlock.scope = statement.mutable_scope();
        colScopedBlock.block = colScopedBlock.scope->mutable_body()->mutable_block();
        return colScopedBlock;
    }
}