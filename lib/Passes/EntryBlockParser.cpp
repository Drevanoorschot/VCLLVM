//
// Created by dre on 11/23/22.
//
#include "Passes/EntryBlockParser.h"

#include <memory>
#include <utility>

namespace llvm {

    EntryBlockParser::EntryBlockParser(std::shared_ptr<AST::Program> p_AST) : p_AST(std::move(p_AST)) {}

    PreservedAnalyses EntryBlockParser::run(Function &F, FunctionAnalysisManager &AM) {
        AST::Function &function = p_AST->getFunction(F.getName().str());
        for (auto &I: F.getEntryBlock()) {
            auto opCode = I.getOpcode();
            switch (opCode) {
                case Instruction::Add:
                case Instruction::Sub:
                case Instruction::Mul:
                case Instruction::SDiv:
                    appendBinExpr(function, I);
                    break;
                case Instruction::Alloca:
                    // Ignoring variable allocations
                    // it's free real estate
                    break;
                case Instruction::Store:
                    appendSimpleAssignment(function, I, STORE);
                    break;
                case Instruction::Load:
                    appendSimpleAssignment(function, I, LOAD);
                    break;
                case Instruction::Ret:
                    appendReturnStatement(function, I);
                    break;
                default:
                    errs() << "Unsupported Opcode: " << I.getOpcodeName() << "\n";
                    exit(EXIT_FAILURE);
            }
        }
        return PreservedAnalyses::all();
    }

    Types::Type EntryBlockParser::determineType(Type *t) {
        switch (t->getTypeID()) {
            case Type::IntegerTyID:
                switch (t->getIntegerBitWidth()) {
                    case 32:
                        return Types::INT;
                    default:
                        errs() << "Unsupported bit width integer\n";
                        exit(EXIT_FAILURE);
                }
            case Type::PointerTyID:
                return determineType(t->getNonOpaquePointerElementType());
            default:
                errs() << "Unsupported type\n";
                exit(EXIT_FAILURE);
        }
    }

    void EntryBlockParser::appendDeclaration(AST::Function &function, Type *llvmType, std::string varName) {
        Types::Type type = determineType(llvmType);
        std::unique_ptr<AST::Identifier> varId = std::make_unique<AST::Identifier>(varName);
        auto varDecl = std::make_unique<AST::VarDecl>(std::move(varId), type);
        function.statements.push_back(std::move(varDecl));
    }

    void EntryBlockParser::appendBinExpr(AST::Function &function, Instruction &I) {
        // insert variable declaration first (we don't support direct assignment in this wonky AST)
        appendDeclaration(function, I.getType(), I.getName().str());
        // now we can assign the expression (assuming no constants are used, just other vars)
        // TODO support constants
        AST::BinOp op;
        switch (I.getOpcode()) {
            case Instruction::Add:
                op = AST::ADD;
                break;
            case Instruction::Sub:
                op = AST::SUB;
                break;
            case Instruction::Mul:
                op = AST::MUL;
                break;
            case Instruction::SDiv:
                op = AST::DIV;
                break;
            default:
                errs() << "Unsupported Opcode: " << I.getOpcodeName() << "\n";
                exit(EXIT_FAILURE);
        }
        auto varName = std::make_unique<AST::Identifier>(I.getName().str());
        auto var1 = std::make_unique<AST::Identifier>(I.getOperand(0)->getName().str());
        auto var2 = std::make_unique<AST::Identifier>(I.getOperand(1)->getName().str());
        auto binExpr = std::make_unique<AST::BinExpression>(
                std::move(var1),
                op,
                std::move(var2)
        );
        auto varAss = std::make_unique<AST::VarAss>(std::move(varName), std::move(binExpr));
        function.statements.push_back(std::move(varAss));
    }

    void
    EntryBlockParser::appendSimpleAssignment(AST::Function &function, Instruction &I, SimpleAssignmentType saType) {
        std::string lhs;
        std::string rhs;
        Type *type;
        switch (saType) {
            case STORE:
                lhs = I.getOperand(0)->getName().str();
                rhs = I.getOperand(1)->getName().str();
                type = I.getOperand(0)->getType();
                break;
            case LOAD:
                lhs = I.getOperand(0)->getName().str();
                rhs = I.getName();
                type = I.getType();
                break;
        }
        appendDeclaration(function, type, rhs);
        auto varAss = std::make_unique<AST::VarAss>(
                std::make_unique<AST::Identifier>(rhs),
                std::make_unique<AST::Identifier>(lhs)
        );
        function.statements.push_back(std::move(varAss));
    }

    void EntryBlockParser::appendReturnStatement(AST::Function &function, Instruction &I) {
        auto returnStat = std::make_unique<AST::ReturnStatement>(
                std::make_unique<AST::Identifier>(I.getOperand(0)->getName().str())
        );
        function.statements.push_back(std::move(returnStat));
    }

}