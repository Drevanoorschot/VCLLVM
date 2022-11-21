//
// Created by dre on 11/18/22.
//

#ifndef VCLLVM_INSTRUCTIONMAPPER_H
#define VCLLVM_INSTRUCTIONMAPPER_H
#include <llvm/IR/PassManager.h>

namespace llvm {

    class InstructionMapper : public PassInfoMixin<InstructionMapper> {
    private:
        std::string test_string;
    public:
        InstructionMapper(std::string test_string);

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    };

}
#endif //VCLLVM_INSTRUCTIONMAPPER_H
