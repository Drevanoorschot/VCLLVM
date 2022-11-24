//
// Created by dre on 11/23/22.
//

#ifndef VCLLVM_ENTRYBLOCKPARSER_H
#define VCLLVM_ENTRYBLOCKPARSER_H

#include <llvm/IR/PassManager.h>
#include "AST/AST.h"

namespace llvm {
    class EntryBlockParser : public PassInfoMixin<EntryBlockParser> {
    private:
        enum SimpleAssignmentType {
            STORE, LOAD
        };
        std::shared_ptr<AST::Program> pAst;
    public:
        explicit EntryBlockParser(std::shared_ptr<AST::Program> pAst);

        PreservedAnalyses run(Function &f, FunctionAnalysisManager &am);

        static types::Type determineType(Type *t);

        static void appendDeclaration(AST::Function &function, Type *llvmType, const std::string& varName);

        static void appendBinExpr(AST::Function &function, Instruction &instr);

        static void appendSimpleAssignment(AST::Function &function, Instruction &instr, SimpleAssignmentType saType);

        static void appendReturnStatement(AST::Function &function, Instruction &instr);
    };

}
#endif //VCLLVM_ENTRYBLOCKPARSER_H
