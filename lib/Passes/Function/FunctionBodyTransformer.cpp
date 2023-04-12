#include "Passes/Function/FunctionBodyTransformer.h"

#include "Passes/Function/FunctionDeclarer.h"
#include "Transform/BlockTransform.h"
#include "Transform/Transform.h"


namespace vcllvm {

    FunctionCursor::FunctionCursor(col::Scope &functionScope, col::Block &functionBody, llvm::LoopInfo &loopInfo) :
            functionScope(functionScope), functionBody(functionBody), loopInfo(loopInfo) {}

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
        return llvmBlock2LabeledColBlock.contains(&llvmBlock);
    }

    LabeledColBlock &FunctionCursor::getOrSetLlvmBlock2LabeledColBlockEntry(BasicBlock &llvmBlock) {
        if (!llvmBlock2LabeledColBlock.contains(&llvmBlock)) {
            col::Label *label = functionBody.add_statements()->mutable_label();
            col::LabelDecl *labelDecl = label->mutable_decl();
            llvm2Col::setColNodeId(labelDecl);
            col::Block *block = label->mutable_stat()->mutable_block();
            LabeledColBlock labeledColBlock = {*label, *block};
            llvmBlock2LabeledColBlock.insert({&llvmBlock, labeledColBlock});
        }
        return llvmBlock2LabeledColBlock.at(&llvmBlock);
    }

    LoopInfo &FunctionCursor::getLoopInfo() {
        return loopInfo;
    }

    FunctionBodyTransformerPass::FunctionBodyTransformerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses FunctionBodyTransformerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        ColScopedFuncBody scopedFuncBody = FAM.getResult<FunctionDeclarer>(F).getAssociatedScopedColFuncBody();
        LoopInfo &loopInfo = FAM.getResult<LoopAnalysis>(F);
        FunctionCursor funcCursor = FunctionCursor(*scopedFuncBody.scope, *scopedFuncBody.block, loopInfo);
        // add function arguments to the variableMap
        for (auto &A: F.args()) {
            funcCursor.addVariableMapEntry(A, FAM.getResult<FunctionDeclarer>(F).getFuncArgMapEntry(A));
        }
        // start recursive block code gen with basic block
        llvm::BasicBlock &entryBlock = F.getEntryBlock();
        llvm2Col::transformLlvmBlock(entryBlock, funcCursor);
        return PreservedAnalyses::all();
    }
}