#include "Passes/Function/FunctionBodyTransformer.h"

#include "Passes/Function/FunctionDeclarer.h"
#include "Passes/Function/FunctionContractDeclarer.h"
#include "Transform/BlockTransform.h"
#include "Transform/Transform.h"
#include "Origin/OriginProvider.h"
#include "Util/Exceptions.h"


namespace vcllvm {
    const std::string SOURCE_LOC = "Passes::Function::FunctionBodyTransformer";

    FunctionCursor::FunctionCursor(col::Scope &functionScope,
                                   col::Block &functionBody,
                                   llvm::Function &llvmFunction,
                                   llvm::FunctionAnalysisManager &FAM) :
            functionScope(functionScope), functionBody(functionBody), llvmFunction(llvmFunction), FAM(FAM) {}

    const col::Scope &FunctionCursor::getFunctionScope() {
        return functionScope;
    }

    void FunctionCursor::addVariableMapEntry(Value &llvmValue, col::Variable &colVar) {
        variableMap.insert({&llvmValue, &colVar});
        // add reference to reference lut of function contract
        col::StringRef *ref = FAM.getResult<FunctionContractDeclarer>(llvmFunction).getAssociatedColFuncContract().add_references();
        ref->set_v1(llvm2Col::getValueName(llvmValue));
        ref->mutable_v2()->set_index(colVar.id());
    }

    col::Variable &FunctionCursor::getVariableMapEntry(Value &llvmValue) {
        return *variableMap.at(&llvmValue);
    }

    bool FunctionCursor::isVisited(BasicBlock &llvmBlock) {
        return llvmBlock2LabeledColBlock.contains(&llvmBlock);
    }

    LabeledColBlock &FunctionCursor::getOrSetLlvmBlock2LabeledColBlockEntry(BasicBlock &llvmBlock) {
        if (!llvmBlock2LabeledColBlock.contains(&llvmBlock)) {
            // create label in buffer
            col::Label *label = functionBody.add_statements()->mutable_label();
            // set label origin
            label->set_origin(llvm2Col::generateLabelOrigin(llvmBlock));
            // create label declaration in buffer
            col::LabelDecl *labelDecl = label->mutable_decl();
            // set label decl origin
            labelDecl->set_origin(llvm2Col::generateLabelOrigin(llvmBlock));
            // set label decl id
            llvm2Col::setColNodeId(labelDecl);
            // create block inside label statement
            col::Block *block = label->mutable_stat()->mutable_block();
            // set block origin
            block->set_origin(llvm2Col::generateBlockOrigin(llvmBlock));
            // add labeled block to the block2block lut
            LabeledColBlock labeledColBlock = {*label, *block};
            llvmBlock2LabeledColBlock.insert({&llvmBlock, labeledColBlock});
        }
        return llvmBlock2LabeledColBlock.at(&llvmBlock);
    }

    LoopInfo &FunctionCursor::getLoopInfo() {
        return FAM.getResult<LoopAnalysis>(llvmFunction);
    }

    LoopInfo &FunctionCursor::getLoopInfo(Function &otherLlvmFunction) {
        return FAM.getResult<LoopAnalysis>(otherLlvmFunction);
    }

    FDResult &FunctionCursor::getFDResult() {
        return FAM.getResult<FunctionDeclarer>(llvmFunction);
    }

    FDResult &FunctionCursor::getFDResult(Function &otherLlvmFunction) {
        return FAM.getResult<FunctionDeclarer>(otherLlvmFunction);
    }

    col::Variable &FunctionCursor::declareVariable(Instruction &llvmInstruction) {
        // create declaration in buffer
        col::Variable *varDecl = functionScope.add_locals();
        // set type of declaration
        try {
            llvm2Col::transformAndSetType(*llvmInstruction.getType(), *varDecl->mutable_t());
        } catch (vcllvm::UnsupportedTypeException &e) {
            std::stringstream errorStream;
            errorStream << e.what() << " in variable declaration.";
            ErrorReporter::addError(SOURCE_LOC, errorStream.str(), llvmInstruction);
        }
        // set id
        llvm2Col::setColNodeId(varDecl);
        // set origin
        varDecl->set_origin(llvm2Col::generateSingleStatementOrigin(llvmInstruction));
        // add to the variable lut
        this->addVariableMapEntry(llvmInstruction, *varDecl);
        return *varDecl;
    }

    col::Assign &FunctionCursor::createAssignmentAndDeclaration(Instruction &llvmInstruction, col::Block &colBlock) {
        col::Variable &varDecl = declareVariable(llvmInstruction);
        return createAssignment(llvmInstruction, colBlock, varDecl);
    }

    col::Assign &FunctionCursor::createAssignment(Instruction &llvmInstruction,
                                                  col::Block &colBlock,
                                                  col::Variable &varDecl) {
        col::Assign *assignment = colBlock.add_statements()->mutable_assign();
        assignment->set_origin(llvm2Col::generateSingleStatementOrigin(llvmInstruction));
        // create local target in buffer and set origin
        col::Local *colLocal = assignment->mutable_target()->mutable_local();
        colLocal->set_origin(llvm2Col::generateAssignTargetOrigin(llvmInstruction));
        // set target to refer to var decl
        colLocal->mutable_ref()->set_index(varDecl.id());
        return *assignment;
    }

    FunctionBodyTransformerPass::FunctionBodyTransformerPass(std::shared_ptr<col::Program> pProgram) :
            pProgram(std::move(pProgram)) {}

    PreservedAnalyses FunctionBodyTransformerPass::run(Function &F, FunctionAnalysisManager &FAM) {
        ColScopedFuncBody scopedFuncBody = FAM.getResult<FunctionDeclarer>(F).getAssociatedScopedColFuncBody();
        FunctionCursor funcCursor = FunctionCursor(*scopedFuncBody.scope, *scopedFuncBody.block, F, FAM);
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