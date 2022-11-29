
#include <utility>
#include <sstream>
#include <iostream>

#include "AST/proto/ast.pb.h"

#include "AST/AST.h"

namespace types {
    std::string toString(types::Type t) {
        switch (t) {
            case POINTER:
                return "POINTER";
            case INT:
                return "INT";
            default:
                std::cerr << "";
                exit(EXIT_FAILURE);
        }
    }
}


namespace AST {

    BinExpression::BinExpression(std::unique_ptr<Expression> lhs, BinOp op, std::unique_ptr<Expression> rhs) :
            lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {}

    std::string BinExpression::toString() {
        auto ret = std::string("BinExpr");
        ret.push_back(op);
        return ret;
    }

    void BinExpression::print(std::ostream &stream) {
        stream << this->toString() << ":(";
        lhs->print(stream);
        stream << ":";
        rhs->print(stream);
        stream << ")";
    }

    Identifier::Identifier(std::string name) : name(std::move(name)) {}


    std::string Identifier::toString() {
        return "ID:" + this->name;
    }

    void Identifier::print(std::ostream &stream) {
        stream << this->name;
    }

    VarDecl::VarDecl(std::unique_ptr<Identifier> varName, types::Type type) :
            type(type), varName(std::move(varName)) {}

    VarDecl::~VarDecl() = default;

    std::string VarDecl::toString() {
        return types::toString(this->type) + ":" + this->varName->name;
    }

    void VarDecl::print(std::ostream &stream) {
        stream << "\t\t" << toString() << "\n";
    }

    VarAss::VarAss(std::unique_ptr<Identifier> varName,
                   std::unique_ptr<Expression> expr) :
            varName(std::move(varName)), expr(std::move(expr)) {}

    std::string VarAss::toString() {
        return "VarAss:" + this->varName->name;
    }

    void VarAss::print(std::ostream &stream) {
        stream << "\t\t" << toString() << ":=";
        this->expr->print(stream);
        stream << "\n";
    }

    ReturnStatement::ReturnStatement(std::unique_ptr<Expression> returnVal) :
            returnVal(std::move(returnVal)) {}

    ReturnStatement::~ReturnStatement() =
    default;

    std::string ReturnStatement::toString() {
        return "Ret";
    }

    void ReturnStatement::print(std::ostream &stream) {
        stream << "\t\t" << toString() << ":";
        this->returnVal->print(stream);
        stream << "\n";
    }


    FunctionParam::FunctionParam(types::Type t, std::unique_ptr<Identifier> id) :
            type(t), paramName(std::move(id)) {}

    std::string FunctionParam::toString() {
        return types::toString(type) + ":" + paramName->name;
    }

    void FunctionParam::print(std::ostream &stream) {
        stream << toString();
    }


    Function::Function(std::string name, std::vector<FunctionParam> params, types::Type returnType) :
            name(std::move(name)),
            params(std::move(params)),
            statements(std::vector<std::unique_ptr<Statement>>()),
            returnType(returnType) {}

    std::string Function::toString() {
        return "Func:" + this->name;
    }

    void Function::print(std::ostream &stream) {
        //func
        stream << toString();
        //params
        stream << "(";
        bool first = true;
        for (auto &p: this->params) {
            if (!first) stream << " ";
            first = false;
            p.print(stream);
        }
        stream << ")";
        // return type
        stream << " -> " << types::toString(returnType);
        //statements
        stream << " {\n";
        for (auto &s: this->statements) {
            s->print(stream);
        }
        stream << "\t}";
    }

    Program::Program() {
        functions = std::vector<Function>();
    }

    std::string Program::toString() {
        return "Program";
    }

    void Program::print(std::ostream &stream) {
        stream << "Program {\n";
        for (auto &function: functions) {
            stream << "\t";
            function.print(stream);
            stream << "\n";
        }
        stream << "}";
    }

    Function &Program::getFunction(const std::string &functionName) {
        for (auto &f: functions) {
            if (f.name == functionName) return f;
        }
        std::cerr << "Cannot find function in program with name \"" << functionName << "\"\n";
        exit(EXIT_FAILURE);
    }

    Statement::~Statement() = default;

    Expression::~Expression() = default;
}