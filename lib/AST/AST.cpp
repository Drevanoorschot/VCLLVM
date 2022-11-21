
#include <utility>

#include "../../include/AST/AST.h"

namespace AST {

    BinExpression::BinExpression(
            std::unique_ptr<Expression> lhs,
            BinOp op,
            std::unique_ptr<Expression> rhs) :
            lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {}


    std::string BinExpression::toString() {
        return &"BinExpr"[this->op];
    }

    Identifier::Identifier(std::string name) : name(std::move(name)) {}

    std::string Identifier::toString() {
        return "ID:" + this->name;
    }


    VarDecl::VarDecl(Identifier lhs, std::unique_ptr<Expression> rhs) :
            lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    std::string VarDecl::toString() {
        return "Var:" + this->lhs.name;
    }

    ReturnStatement::ReturnStatement(std::unique_ptr<Expression> returnVal) :
            returnVal(std::move(returnVal)) {}

    std::string ReturnStatement::toString() {
        return "ReturnStat";

    }

    Function::Function(std::string name, std::vector<Identifier> params) :
            name(std::move(name)),
            params(std::move(params)),
            statements(std::vector<Statement>()) {}

    std::string Function::toString() {
        return "Func:" + this->name;
    }


    std::string Program::toString() {
        return "Program";
    }

    Program::Program() {
        functions = std::vector<Function>();
    }

}