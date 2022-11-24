//
// Created by dre on 11/23/22.
//
#include <utility>

#include "Passes/VariableNamer.h"

namespace llvm {
    VariableNamer::VariableNamer(std::shared_ptr<AST::Program> pAst) : pAst(std::move(pAst)) {}

    PreservedAnalyses VariableNamer::run(Function &f, FunctionAnalysisManager &am) {
        int varCounter = 0;
        int argCounter = 0;
        for (auto &arg: f.args()) {
            if (!arg.hasName()) {
                arg.setName("arg" + itostr(argCounter));
                argCounter++;
            }
        }
        // TODO add support for multiblock at some point
        for (auto &instr: f.getEntryBlock()) {
            if (!instr.hasName()) {
                instr.setName("var" + itostr(varCounter));
                varCounter++;
            }
        }
        return PreservedAnalyses::all();
    }
}