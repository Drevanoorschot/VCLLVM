//
// Created by dre on 11/18/22.
//
#include <utility>

#include "../../include/Passes/InstructionMapper.h"

using namespace llvm;

InstructionMapper::InstructionMapper(std::string test_string) {
    this->test_string = std::move(test_string);
}

PreservedAnalyses InstructionMapper::run(Function &F, FunctionAnalysisManager &AM) {
    errs() << "Test String test: " << test_string;
    int block_counter = 0;
    for (Function::iterator b = F.begin(), be = F.end(); b != be; ++b) {
        errs() << "block 0 in " << F.getName() << ":\n";
        block_counter++;
        for(BasicBlock::iterator i = b->begin(), ie = b->end(); i != ie; ++i) {
            errs() << "\t" << i->getOpcodeName() << "\n";
        }
    }
    return PreservedAnalyses::all();
}