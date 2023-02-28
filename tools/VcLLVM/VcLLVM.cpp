#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>

#include "col.pb.h"
#include "Passes/FunctionDeclarer.h"

#include <iostream>
#include <memory>

using namespace llvm;

using namespace vct::col::serialize;

GlobalDeclaration sampleCol() {
    // class
    GlobalDeclaration classDeclaration = GlobalDeclaration();
    classDeclaration.set_id(reinterpret_cast<int64_t>(&classDeclaration));
    VctClass *vctClass = classDeclaration.mutable_vct_class();
    BooleanValue *lockInvariant = vctClass->mutable_intrinsic_lock_invariant()->mutable_boolean_value();
    lockInvariant->set_value(true);
    // class>method
    ClassDeclaration *methodDeclaration = vctClass->add_declarations();
    methodDeclaration->set_id(reinterpret_cast<int64_t>(methodDeclaration));
    InstanceMethod *method = methodDeclaration->mutable_instance_method();
    // class>method>return_type
    TBool returnType = method->mutable_return_type()->t_bool();
    // class>method>body
    Block *body = method->mutable_body()->mutable_scope()->mutable_body()->mutable_block();
    Return *returnStatement = body->add_statements()->mutable_return_();
    BooleanValue *returnValue = returnStatement->mutable_result()->mutable_boolean_value();
    returnValue->set_value(true);
    // class>method>inline
    method->set_inline_(false);
    // class>method>pure
    method->set_pure(false);
    // class>method>contract
    ApplicableContract *contract = method->mutable_contract();
    // class>method>contract>precondition
    UnitAccountedPredicate *precondition = contract->mutable_requires_()->mutable_unit_accounted_predicate();
    BooleanValue *prePred = precondition->mutable_pred()->mutable_boolean_value();
    prePred->set_value(true);
    // class>method>contract>postcondition
    UnitAccountedPredicate *postcondition = contract->mutable_ensures()->mutable_unit_accounted_predicate();
    Ref *postRefResult = postcondition->mutable_pred()->mutable_result()->mutable_applicable();
    postRefResult->set_id(methodDeclaration->id());
    // class>method>contract>context_everywhere
    BooleanValue *contextEverywhere = contract->mutable_context_everywhere()->mutable_boolean_value();
    contextEverywhere->set_value(true);
    return classDeclaration;
}

static cl::opt<std::string> inputFileName{"",
                                          cl::desc{"Module to analyze"},
                                          cl::value_desc{"IR filename"},
                                          cl::Positional};
static cl::opt<bool> testCol{"sample-col",
                             cl::desc{"Output a sample COL buffer for testing"}};

int main(int argc, char **argv) {
    cl::ParseCommandLineOptions(argc, argv);
    // sample mode
    if (testCol.getValue()) {
        std::cout << sampleCol().SerializeAsString() << '\n';
        return 0;
    }
    // parse mode
    if (inputFileName.empty()) {
        errs() << "no input file given" << '\n';
        return 1;
    }
    LLVMContext context;
    SMDiagnostic smDiag;
    auto pModule = parseIRFile(inputFileName, smDiag, context);
    if (!pModule) {
        errs() << smDiag.getMessage() << "\n";
        return 1;
    }
    Module *module = pModule.get();
    module->dump();
    return 0;
}