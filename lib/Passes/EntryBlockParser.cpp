//
// Created by dre on 11/23/22.
//
#include "Passes/EntryBlockParser.h"

#include <memory>
#include <utility>

namespace llvm {

    EntryBlockParser::EntryBlockParser(std::shared_ptr<AST::Program> pAst) : pAst(std::move(pAst)) {}

    PreservedAnalyses EntryBlockParser::run(Function &f, FunctionAnalysisManager &am) {
        AST::Function &function = pAst->getFunction(f.getName().str());
        for (auto &instr: f.getEntryBlock()) {
            auto opCode = instr.getOpcode();
            switch (opCode) {
                case Instruction::Add:
                case Instruction::Sub:
                case Instruction::Mul:
                case Instruction::SDiv:
                    appendBinExpr(function, instr);
                    break;
                case Instruction::Alloca:
                    // Ignoring variable allocations
                    // it's free real estate
                    break;
                case Instruction::Store:
                    appendSimpleAssignment(function, instr, STORE);
                    break;
                case Instruction::Load:
                    appendSimpleAssignment(function, instr, LOAD);
                    break;
                case Instruction::Ret:
                    appendReturnStatement(function, instr);
                    break;
                default:
                    errs() << "Unsupported Opcode: " << instr.getOpcodeName() << "\n";
                    exit(EXIT_FAILURE);
            }
        }
        return PreservedAnalyses::all();
    }

    types::Type EntryBlockParser::determineType(Type *t) {
        switch (t->getTypeID()) {
            case Type::IntegerTyID:
                switch (t->getIntegerBitWidth()) {
                    case 32:
                        return types::INT;
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

    void EntryBlockParser::appendDeclaration(AST::Function &function, Type *llvmType, const std::string& varName) {
        types::Type type = determineType(llvmType);
        std::unique_ptr<AST::Identifier> varId = std::make_unique<AST::Identifier>(varName);
        auto varDecl = std::make_unique<AST::VarDecl>(std::move(varId), type);
        function.statements.push_back(std::move(varDecl));
    }

    void EntryBlockParser::appendBinExpr(AST::Function &function, Instruction &instr) {
        // insert variable declaration first (we don't support direct assignment in this wonky AST)
        appendDeclaration(function, instr.getType(), instr.getName().str());
        // now we can assign the expression (assuming no constants are used, just other vars)
        // TODO support constants
        AST::BinOp op;
        switch (instr.getOpcode()) {
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
                errs() << "Unsupported Opcode: " << instr.getOpcodeName() << "\n";
                exit(EXIT_FAILURE);
        }
        auto varName = std::make_unique<AST::Identifier>(instr.getName().str());
        auto var1 = std::make_unique<AST::Identifier>(instr.getOperand(0)->getName().str());
        auto var2 = std::make_unique<AST::Identifier>(instr.getOperand(1)->getName().str());
        auto binExpr = std::make_unique<AST::BinExpression>(
                std::move(var1),
                op,
                std::move(var2)
        );
        auto varAss = std::make_unique<AST::VarAss>(std::move(varName), std::move(binExpr));
        function.statements.push_back(std::move(varAss));
    }

    void
    EntryBlockParser::appendSimpleAssignment(AST::Function &function, Instruction &instr, SimpleAssignmentType saType) {
        std::string lhs;
        std::string rhs;
        Type *type;
        switch (saType) {
            case STORE:
                lhs = instr.getOperand(0)->getName().str();
                rhs = instr.getOperand(1)->getName().str();
                type = instr.getOperand(0)->getType();
                break;
            case LOAD:
                lhs = instr.getOperand(0)->getName().str();
                rhs = instr.getName();
                type = instr.getType();
                break;
        }
        appendDeclaration(function, type, rhs);
        auto varAss = std::make_unique<AST::VarAss>(
                std::make_unique<AST::Identifier>(rhs),
                std::make_unique<AST::Identifier>(lhs)
        );
        function.statements.push_back(std::move(varAss));
    }

    void EntryBlockParser::appendReturnStatement(AST::Function &function, Instruction &instr) {
        auto returnStat = std::make_unique<AST::ReturnStatement>(
                std::make_unique<AST::Identifier>(instr.getOperand(0)->getName().str())
        );
        function.statements.push_back(std::move(returnStat));
    }

}