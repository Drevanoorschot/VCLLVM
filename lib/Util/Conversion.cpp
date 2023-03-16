#include <llvm/IR/DerivedTypes.h>
#include "Util/Conversion.h"
#include "Util/Exceptions.h"

namespace col = vct::col::serialize;


/**
 * Utility function that converts LLVM types to col types
 * @param type
 */
void llvm2Col::convertAndSetType(llvm::Type &llvmType, col::Type &colType) {
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

col::Block &llvm2Col::setAndReturnScopedBlock(col::Statement &statement) {
    return *statement.mutable_scope()->mutable_body()->mutable_block();
}