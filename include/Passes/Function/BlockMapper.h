#ifndef VCLLVM_BLOCKMAPPER_H
#define VCLLVM_BLOCKMAPPER_H

#include <llvm/IR/PassManager.h>
#include "Transform/Transform.h"

#include "col.pb.h"

#include <variant>


/**
 * Analysis Pass that maps llvm blocks to COL nodes (branches, loops, blocks and expressions)
 * It operates by recursively following the CFG. Effectively, this means recursively following the terminator
 * instructions of blocks where:
 * <ul>
 * <li>a return block are leaf nodes of the recursion</li>
 * <li>a branch block with branches is converted to a COL branch</li>
 * <li>loop block(s) are converted to COL LLVMLoops (determined by querying loop header status of branch block).</li>
 * <li>a so called "jump block" (i.e. single operand branch blocks that jump to another block). This can either be a latch
 * or an entry block to a branch or loop</li>
 * </ul>
 * It also takes care of scoping of loop and if bodies in the COL AST.
 */
namespace vcllvm {
    using namespace llvm;
    namespace col = vct::col::serialize;

    class BMAResult {
        friend class BlockMapper;

    private:
        /**
         *
         */
        std::unordered_map<llvm::BasicBlock *, col::Block *> llvmBlock2ColBlock;

        void addLlvmBlock2ColBlockEntry(llvm::BasicBlock &llvmBlock, col::Block &colBlock);
        // TODO conditional mapping
        // TODO loop mapping
    public:
        std::unordered_map<llvm::BasicBlock *, col::Block *> getLlvmBlock2ColBlock();
    };

    class BlockMapper : public AnalysisInfoMixin<BlockMapper> {
        friend AnalysisInfoMixin<BlockMapper>;
        static AnalysisKey Key;
    private:
        std::shared_ptr<col::Program> pProgram;
        BMAResult result;

        /**
         * Base function for recursing through the CFG
         * Calls one of the appropriate block handle functions
         * @param llvmBlock
         * @param colBlock
         */
        void handleBlock(llvm::BasicBlock &llvmBlock,
                         col::Block &colBlock,
                         llvm::FunctionAnalysisManager &FAM);

        /**
         * Does the following:
         * <ul>
         * <li>Adds an unscoped COL block to the current <code>colScopedBlock</code></li>
         * <li>Links <code>llvmBlock</code> to the created COL block</li>
         * </ul>
         *
         * As this block has no successor blocks, it functions as leaf node of the BlockMapper recursion tree.
         * @param llvmBlock
         * @param colBlock
         */
        void handleReturnBlock(llvm::BasicBlock &llvmBlock,
                               col::Block &colBlock);

        void handleLoop(llvm::BasicBlock &llvmBlock,
                        col::Block &colBlock,
                        llvm::FunctionAnalysisManager &FAM);

        void handleBranch(llvm::BasicBlock &llvmBlock,
                          col::Block &colBlock,
                          llvm::FunctionAnalysisManager &FAM);

        /**
         * Does the following:
         * <ul>
         * <li>Adds an unscoped COL block to the current <code>colScopedBlock</code></li>
         * <li>Links <code>llvmBlock</code> to the created COL block</li>
         * <li>Stops recursing further into the CFG if the block turns out to be a latch</li>
         * </ul>
         * @param llvmBlock
         * @param colBlock
         * @param FAM
         */
        void handleJump(llvm::BasicBlock &llvmBlock,
                        col::Block &colBlock,
                        llvm::FunctionAnalysisManager &FAM);
        /**
         * Simple helper function handling the identical behaviour of
         * <code>handleRetBlock</code> and <code>handleJump</code>
         * @param llvmBlock
         * @param colBlock
         */
        void handleBlock2Block(llvm::BasicBlock &llvmBlock,
                               col::Block &colBlock);

    public:
        using Result = BMAResult;

        explicit BlockMapper(std::shared_ptr<col::Program> pProgram);

        BMAResult run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);
    };

}

#endif //VCLLVM_BLOCKMAPPER_H
