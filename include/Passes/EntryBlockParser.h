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
        std::shared_ptr<AST::Program> p_AST;
    public:
        explicit EntryBlockParser(std::shared_ptr<AST::Program> p_AST);

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

        static Types::Type determineType(Type *t);

        static void appendDeclaration(AST::Function &function, Type *llvmType, std::string varName);

        static void appendBinExpr(AST::Function &function, Instruction &I);

        static void appendSimpleAssignment(AST::Function &function, Instruction &I, SimpleAssignmentType saType);

        static void appendReturnStatement(AST::Function &function, Instruction &I);
    };

}
#endif //VCLLVM_ENTRYBLOCKPARSER_H
