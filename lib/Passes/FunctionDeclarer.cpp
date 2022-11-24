//
// Created by dre on 11/21/22.
//
#include <memory>
#include <utility>

#include "Passes/FunctionDeclarer.h"

namespace llvm {
    FunctionDeclarer::FunctionDeclarer(std::shared_ptr<AST::Program> pAst) : pAst(std::move(pAst)) {}

    PreservedAnalyses FunctionDeclarer::run(Function &f, FunctionAnalysisManager &am) {
        std::vector<AST::FunctionParam> params = std::vector<AST::FunctionParam>();
        for (int i = 0; i < f.arg_size(); i++) {
            auto arg = f.getArg(i);
            types::Type type;
            std::string paramName = arg->getName().str();
            auto typeId = f.getArg(i)->getType()->getTypeID();
            switch (typeId) {
                case Type::IntegerTyID:
                    type = types::INT;
                    break;
                default:
                    errs() << "Unsupported Parameter Type: " << typeId;
                    exit(EXIT_FAILURE);
            }
            params.emplace_back(
                    type,
                    std::make_unique<AST::Identifier>(paramName));
        }
        types::Type returnType;
        switch (f.getReturnType()->getTypeID()) {
            case Type::IntegerTyID:
                returnType = types::INT;
                break;
            default:
                errs() << "Unsupported Return Type: " << f.getReturnType()->getTypeID();
                exit(EXIT_FAILURE);
        }
        pAst->functions.emplace_back(
                f.getName().str(),
                std::move(params),
                returnType);
        return PreservedAnalyses::all();
    }
}