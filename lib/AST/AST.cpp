
#include <utility>
#include <sstream>
#include <iostream>

#include "AST/AST.h"

namespace Types {
    std::string toString(Types::Type t) {
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

    std::string BinExpression::toString() {
        return &"BinExpr"[this->op];
    }

    void BinExpression::print(std::ostream &stream) {
        stream << "STUB!"; //TODO STUB implementation
    }

    Identifier::Identifier(std::string name) : name(std::move(name)) {}


    std::string Identifier::toString() {
        return "ID:" + this->name;
    }

    void Identifier::print(std::ostream &stream) {
        stream << "STUB!"; //TODO STUB implementation
    }

    Type::Type(Types::Type type) : type(type) {}

    std::string Type::toString() {
        return "Type:" + Types::toString(type);
    }

    void Type::print(std::ostream &stream) {
        stream << "STUB!"; //TODO STUB implementation
    }

    VarDecl::VarDecl(std::unique_ptr<Identifier> var_name, Type type) :
            type(std::move(type)), var_name(std::move(var_name)) {}

    VarDecl::~VarDecl() =
    default;

    std::string VarDecl::toString() {
        return Types::toString(this->type.type) + ":" + this->var_name->name;
    }

    void VarDecl::print(std::ostream &stream) {
        stream << "STUB!"; //TODO STUB implementation
    }

    VarAss::VarAss(std::unique_ptr<Identifier>
                   var_name, std::string
                   literal) :
            var_name(std::move(var_name)), literal(std::move(literal)) {}

    std::string VarAss::toString() {
        return "VarAss";
    }

    ReturnStatement::ReturnStatement(std::unique_ptr<Expression>
                                     returnVal) :
            return_val(std::move(returnVal)) {}

    ReturnStatement::~ReturnStatement() =
    default;

    std::string ReturnStatement::toString() {
        return "ReturnStat";
    }

    void ReturnStatement::print(std::ostream &stream) {
        stream << "STUB!"; //TODO STUB implementation
    }


    FunctionParam::FunctionParam(std::unique_ptr<Type> t, std::unique_ptr<Identifier> id) :
            type(std::move(t)), param_name(std::move(id)) {}

    std::string FunctionParam::toString() {
        return Types::toString(type->type) + ":" + param_name->name;
    }

    void FunctionParam::print(std::ostream &stream) {
        stream << toString(); //TODO STUB implementation
    }


    Function::Function(std::string name, std::vector<FunctionParam> params, std::unique_ptr<Type> return_type) :
            name(std::move(name)),
            params(std::move(params)),
            statements(std::vector<Statement>()),
            return_type(std::move(return_type)) {}

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

    Statement::~Statement() = default;
}