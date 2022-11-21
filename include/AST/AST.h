//
// Created by dre on 11/18/22.
//

#ifndef VCLLVM_AST_H
#define VCLLVM_AST_H

#include <string>
#include <vector>
#include <memory>

namespace AST {
    class Node {
    public:
        virtual std::string toString() = 0;
    protected:
        Node() = default;
    };

    class Statement : public Node {
    protected:
        Statement() = default;
    public:
        virtual ~Statement() = 0;
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


        std::unique_ptr<Expression> lhs;
        BinOp op;
        std::unique_ptr<Expression> rhs;
    };


    class Identifier : public Expression {
    public:
        explicit Identifier(std::string name);

        std::string toString() override;


        std::string name;
    };

    class VarDecl : public Statement {
    public:
        VarDecl(Identifier lhs, std::unique_ptr<Expression> rhs);

        std::string toString() override;

        Identifier lhs;
        std::unique_ptr<Expression> rhs;
    };

    class ReturnStatement : public Statement {
    public:
        explicit ReturnStatement(std::unique_ptr<Expression> returnVal);

        std::string toString() override;
        std::unique_ptr<Expression> returnVal;
    };

    class Function : public Node {
    public:
        Function(std::string name, std::vector<Identifier> params);

        std::string toString() override;

        std::string name;
        std::vector<Identifier> params;
        std::vector<Statement> statements;
    };

    class Program : public Node {
    public:
        Program();

        std::string toString() override;

        std::vector<Function> functions;
    };

#endif //VCLLVM_AST_H
}