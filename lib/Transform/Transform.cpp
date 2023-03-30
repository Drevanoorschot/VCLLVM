#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

#include "Transform/Transform.h"
#include "Util/Exceptions.h"

#include "Passes/Function/FunctionBodyTransformer.h"


namespace col = vct::col::serialize;


/**
 * Utility function that converts LLVM types to col types
 * @param type
 */
namespace llvm2Col {
    void transformAndSetType(llvm::Type &llvmType, col::Type &colType) {
        // TODO refactor out exceptions
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


    void transformAndSetExpr(llvm::Value &llvmValue, col::Expr &colExpr, vcllvm::FunctionCursor &functionCursor) {
        auto llvmConst = llvm::dyn_cast<llvm::Constant>(&llvmValue);
        if (llvmConst != nullptr) {
            transformAndSetConstExpr(*llvmConst, colExpr);
        } else {
            transformAndSetVarExpr(llvmValue, colExpr, functionCursor);
        }

    }

    void transformAndSetVarExpr(llvm::Value &llvmValue, col::Expr &colExpr, vcllvm::FunctionCursor &functionCursor) {
        col::Variable colVar = functionCursor.getVariableMapEntry(llvmValue);
        colExpr.mutable_local()->mutable_ref()->set_index(colVar.id());
    }

    void transformAndSetConstExpr(llvm::Constant &llvmConstant, col::Expr &colExpr) {
        llvm::Type *constType = llvmConstant.getType();
        switch (llvmConstant.getType()->getTypeID()) {
            case llvm::Type::IntegerTyID:
                if (constType->getIntegerBitWidth() == 1) {
                    colExpr.mutable_boolean_value()->set_value(llvmConstant.isOneValue());
                } else {
                    llvm::APInt apInt = llvmConstant.getUniqueInteger();
                    transformAndSetIntegerValue(*colExpr.mutable_integer_value(), apInt);
                }
                break;
            default:
                std::string errCtx;
                llvm::raw_string_ostream(errCtx) << llvmConstant;
                std::stringstream errorStream;
                errorStream << "Unknown constant \"" << errCtx << '\"';
                vcllvm::ErrorReporter::addError("Transform::Transform", errorStream.str());
        }
    }

    void transformAndSetIntegerValue(col::IntegerValue &colIntegerValue, llvm::APInt &apInt) {
        // TODO works for "small" signed and unsigned numbers, may break for values > 2^64
        std::vector<u_int64_t> byteVector;
        for (int i = 0; i < apInt.getNumWords(); i++) {
            byteVector.push_back(apInt.getRawData()[i]);
        }
        colIntegerValue.mutable_value()->set_data(byteVector.data(), byteVector.size());
    }
}