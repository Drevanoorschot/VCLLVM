#ifndef VCLLVM_FUNCTIONINSTRUCTIONTRANSFORMER_H
#define VCLLVM_FUNCTIONINSTRUCTIONTRANSFORMER_H

#include <llvm/IR/Instruction.h>
/**
 * Pass that transforms instructions to COL expressions and statements.
 * It starts at the first instruction of the functions entry block and transformers subsequent instructions
 * based on the current state of the function.
 * TODO This gonna be a big boy class, think about how this class could potentially be split up more effectively.
 */

namespace vcllvm {
    using namespace llvm;

    class FunctionInstructionTransformerPass {

    };
}
#endif //VCLLVM_FUNCTIONINSTRUCTIONTRANSFORMER_H
