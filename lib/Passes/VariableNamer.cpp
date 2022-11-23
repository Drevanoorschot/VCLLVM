//
// Created by dre on 11/23/22.
//
#include <utility>

#include "../../include/Passes/VariableNamer.h"

namespace llvm {
    VariableNamer::VariableNamer(std::shared_ptr<AST::Program> p_AST) : p_AST(std::move(p_AST)) {}

    PreservedAnalyses VariableNamer::run(Function &F, FunctionAnalysisManager &AM) {
        int var_counter = 0;
        int arg_counter = 0;
        for (auto &A: F.args()) {
            if (!A.hasName()) {
                A.setName("arg" + itostr(arg_counter));
                arg_counter++;
            }
        }
        // TODO add support for multiblock at some point
        for (auto &I: F.getEntryBlock()) {
            if (!I.hasName()) {
                I.setName("var" + itostr(var_counter));
                var_counter++;
            }
        }
        return PreservedAnalyses::all();
    }
}