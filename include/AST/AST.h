//
// Created by dre on 11/18/22.
//

#ifndef VCLLVM_AST_H
#define VCLLVM_AST_H

#include <string>
#include <vector>
#include <memory>

namespace types {
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
        virtual ~Statement() = 0;
    };

    class Expression : public Node {
    protected:
        Expression() = default;
    public:
        virtual ~Expression() = 0;
    };

    enum BinOp {
        ADD = '+', SUB = '-', MUL = '*', DIV = '/'
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

    class VarDecl : public Statement {
    public:
        VarDecl(std::unique_ptr<Identifier> varName, types::Type type);

        ~VarDecl() override;

        std::string toString() override;

        void print(std::ostream &stream) override;

        types::Type type;
        std::unique_ptr<Identifier> varName;
    };

    class VarAss : public Statement {
    public:
        VarAss(std::unique_ptr<Identifier> varName, std::unique_ptr<Expression> expr);

        std::string toString() override;

        void print(std::ostream &stream) override;


        std::unique_ptr<Identifier> varName;
        std::unique_ptr<Expression> expr;

    };

    class ReturnStatement : public Statement {
    public:
        explicit ReturnStatement(std::unique_ptr<Expression> returnVal);

        ~ReturnStatement() override;

        std::string toString() override;

        void print(std::ostream &stream) override;

        std::unique_ptr<Expression> returnVal;
    };

    class FunctionParam : Node {
    public:
        FunctionParam(types::Type t, std::unique_ptr<Identifier> id);

        std::string toString() override;

        void print(std::ostream &stream) override;


        types::Type type;
        std::unique_ptr<Identifier> paramName;
    };

    class Function : public Node {
    public:
        Function(std::string name, std::vector<FunctionParam> params, types::Type returnType);

        std::string toString() override;

        void print(std::ostream &stream) override;


        std::string name;
        std::vector<FunctionParam> params;
        std::vector<std::unique_ptr<Statement>> statements;
        types::Type returnType;
    };

    class Program : public Node {
    public:
        Program();

        std::string toString() override;

        void print(std::ostream &stream) override;

        std::vector<Function> functions;

        Function &getFunction(const std::string& functionName);
    };
}
#endif //VCLLVM_AST_H
