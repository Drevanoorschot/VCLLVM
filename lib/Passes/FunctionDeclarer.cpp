//
// Created by dre on 11/21/22.
//
#include <memory>
#include <utility>

#include "../../include/Passes/FunctionDeclarer.h"

namespace llvm {
    FunctionDeclarer::FunctionDeclarer(std::shared_ptr<AST::Program> p_AST) : p_AST(std::move(p_AST)) {}

    PreservedAnalyses FunctionDeclarer::run(Function &F, FunctionAnalysisManager &AM) {
        std::vector<AST::FunctionParam> params = std::vector<AST::FunctionParam>();
        F.getEntryBlock().front().dump();
        for (int i = 0; i < F.arg_size(); i++) {
            auto arg = F.getArg(i);
            Types::Type type;
            std::string param_name = arg->getName().str();
            auto type_id = F.getArg(i)->getType()->getTypeID();
            switch (type_id) {
                case Type::IntegerTyID:
                    type = Types::INT;
                    break;
                default:
                    errs() << "Unsupported Parameter Type: " << type_id;
                    exit(EXIT_FAILURE);
            }
            params.emplace_back(
                    std::make_unique<AST::Type>(type),
                    std::make_unique<AST::Identifier>(param_name));
        }
        Types::Type return_type;
        switch (F.getReturnType()->getTypeID()) {
            case Type::IntegerTyID:
                return_type = Types::INT;
                break;
            default:
                errs() << "Unsupported Return Type: " << F.getReturnType()->getTypeID();
                exit(EXIT_FAILURE);
        }
        p_AST->functions.emplace_back(
                F.getName().str(),
                std::move(params),
                std::make_unique<AST::Type>(return_type));
        return PreservedAnalyses::all();
    }
}