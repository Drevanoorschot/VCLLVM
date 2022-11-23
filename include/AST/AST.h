//
// Created by dre on 11/18/22.
//

#ifndef VCLLVM_AST_H
#define VCLLVM_AST_H

#include <string>
#include <vector>
#include <memory>

namespace Types {
    enum Type {
        POINTER, INT
    };
    std::string toString(Type t);
}

namespace AST {
    class Node {
    public:
        virtual std::string toString() = 0;

        virtual void print(std::ostream &stream) = 0;

    protected:
        Node() = default;
    };

    class Statement : public Node {
    protected:
        Statement() = default;

    public:
        virtual ~Statement() = 0; //TODO figure this out
    };

    class Expression : public Node {
    protected:
        Expression() = default;
    };

    enum BinOp {
        ADD = '+', SUB = '-', MULT = '*', DIV = '/'
    };

    class BinExpression : public Expression {
    public:
        BinExpression(std::unique_ptr<Expression> lhs,
                      BinOp op,
                      std::unique_ptr<Expression> rhs);

        std::string toString() override;

        void print(std::ostream &stream) override;


        std::unique_ptr<Expression> lhs;
        BinOp op;
        std::unique_ptr<Expression> rhs;
    };


    class Identifier : public Expression {
    public:
        explicit Identifier(std::string name);

        std::string toString() override;

        void print(std::ostream &stream) override;


        std::string name;
    };

    class Type : public Node {
    public:
        explicit Type(Types::Type type);

        std::string toString() override;

        void print(std::ostream &stream) override;


        Types::Type type;
    };

    class VarDecl : public Statement {
    public:
        VarDecl(std::unique_ptr<Identifier> var_name, Type type);

        ~VarDecl() override;

        std::string toString() override;

        void print(std::ostream &stream) override;

        Type type;
        std::unique_ptr<Identifier> var_name;
    };

    class VarAss : public Statement {
        VarAss(std::unique_ptr<Identifier> var_name, std::string literal);

        std::string toString() override;

        std::unique_ptr<Identifier> var_name;
        std::string literal;
    };

    class ReturnStatement : public Statement {
    public:
        explicit ReturnStatement(std::unique_ptr<Expression> returnVal);

        ~ReturnStatement() override;

        std::string toString() override;

        void print(std::ostream &stream) override;

        std::unique_ptr<Expression> return_val;
    };

    class FunctionParam : Node {
    public:
        FunctionParam(std::unique_ptr<Type> t, std::unique_ptr<Identifier> id);

        std::string toString() override;

        void print(std::ostream &stream) override;


        std::unique_ptr<Type> type;
        std::unique_ptr<Identifier> param_name;
    };

    class Function : public Node {
    public:
        Function(std::string name, std::vector<FunctionParam> params, std::unique_ptr<Type> return_type);

        std::string toString() override;

        void print(std::ostream &stream) override;


        std::string name;
        std::vector<FunctionParam> params;
        std::vector<Statement> statements;
        std::unique_ptr<Type> return_type;
    };

    class Program : public Node {
    public:
        Program();

        std::string toString() override;

        void print(std::ostream &stream) override;

        std::vector<Function> functions;
    };
}
#endif //VCLLVM_AST_H
