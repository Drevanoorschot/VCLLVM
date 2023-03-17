#ifndef VCLLVM_BLOCKMAPPER_H
#define VCLLVM_BLOCKMAPPER_H

#include <llvm/IR/PassManager.h>
#include "Util/Conversion/Conversion.h"

#include "col.pb.h"

#include <variant>


/**
 * Analysis Pass that maps llvm blocks to COL nodes (branches, loops, blocks and expressions)
 * It operates by recursively following the terminator instructions of blocks where
 * - return blocks are leaf nodes of the recursion
 * - branch blocks are handled as either conditionals or loops (determined by following branches).
 * It also takes care of scoping of loop and if bodies in the COL AST.
 */
namespace vcllvm {
    using namespace llvm;
    namespace col = vct::col::serialize;

    class BMAResult {
        friend class BlockMapper;

    private:
        std::unordered_map<llvm::BasicBlock *, llvm2Col::ColScopedBlock> retBlock2ColScopedBlock;

        void addRetBlock2ColScopedBlockEntry(llvm::BasicBlock &llvmBlock, llvm2Col::ColScopedBlock colScopedBlock);
        // TODO conditional mapping
        // TODO loop mapping
    public:
        std::unordered_map<llvm::BasicBlock *, llvm2Col::ColScopedBlock> getRetBlock2ColScopedBlock();
    };

    class BlockMapper : public AnalysisInfoMixin<BlockMapper> {
        friend AnalysisInfoMixin<BlockMapper>;
        static AnalysisKey Key;
    private:
        std::shared_ptr<col::Program> pProgram;
        BMAResult result;

        /**
         * Base function for recursion block recursion tree
         * Calls one of the appropiate block handle functions
         * @param llvmBlock
         * @param colScopedBlock
         */
        void handleBlock(llvm::BasicBlock &llvmBlock, llvm2Col::ColScopedBlock colScopedBlock);

        /**
         * Directly links the provided llvm block with the provided col Block
         * Leaf function of recursion block mapper tree.
         * @param llvmBlock
         * @param colScopedBlock
         */
        void handleReturnBlock(llvm::BasicBlock &llvmBlock, llvm2Col::ColScopedBlock colScopedBlock);

    public:
        using Result = BMAResult;

        explicit BlockMapper(std::shared_ptr<col::Program> pProgram);

        BMAResult run(Function &F, FunctionAnalysisManager &FAM);
    };

}

#endif //VCLLVM_BLOCKMAPPER_H
