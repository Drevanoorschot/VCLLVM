#include "Passes/Function/FunctionBodyTransformer.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Transform/BlockTransform.h"
#include "Transform/Transform.h"

namespace vcllvm {

    FunctionCursor::FunctionCursor(col::Scope &functionScope) :
            functionScope(functionScope) {}

    col::Scope &FunctionCursor::getFunctionScope() {
        return functionScope;
    }

    void FunctionCursor::addVariableMapEntry(Value &llvmValue, col::Variable &colVar) {
        variableMap.insert({&llvmValue, &colVar});
    }

    col::Variable &FunctionCursor::getVariableMapEntry(Value &llvmValue) {
        return *variableMap.at(&llvmValue);
    }

    bool FunctionCursor::isVisited(BasicBlock &llvmBlock) {
        return llvmBlock2ColBlock.contains(&llvmBlock);
    }


    col::Block &FunctionCursor::getLlvmBlock2ColBlockEntry(BasicBlock &llvmBlock) {
        return *llvmBlock2ColBlock.at(&llvmBlock);
    }

    col::Block & FunctionCursor::setAndMapLlvmBlock2ColBlock(BasicBlock &llvmBlock, col::Block &parentColBlock) {
        col::Block *colBlock = parentColBlock.add_statements()->mutable_block();
        llvmBlock2ColBlock.insert({&llvmBlock, colBlock});
        return *colBlock;
    }

    FunctionBodyTransformerPass::FunctionBodyTransformerPass(std::shared_ptr<col::Program> pProgram):
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses FunctionBodyTransformerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        ColScopedFuncBody scopedFuncBody = FAM.getResult<FunctionDeclarer>(F).getAssociatedScopedColFuncBody();
        FunctionCursor funcCursor = FunctionCursor(*scopedFuncBody.scope);
        // add function arguments to the variableMap
        for(auto &A : F.args()) {
            funcCursor.addVariableMapEntry(A, FAM.getResult<FunctionDeclarer>(F).getFuncArgMapEntry(A));
        }
        // start recursive block code gen with basic block
        llvm::BasicBlock &entryBlock = F.getEntryBlock();
        llvm2Col::transformLlvmBlock(funcCursor, *scopedFuncBody.block, entryBlock);
        return PreservedAnalyses::all();
    }
}