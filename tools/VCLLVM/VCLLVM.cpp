#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CommandLine.h>

#include "col.pb.h"
#include "Passes/Function/FunctionContractDeclarer.h"
#include "Passes/Function/FunctionDeclarer.h"
#include "Passes/Function/PureAssigner.h"
#include "Util/Conversion.h"
#include "Util/Exceptions.h"

#include <iostream>
#include <memory>

namespace col = vct::col::serialize;

col::Program sampleCol(bool returnBool) {
    col::Program program = col::Program();

    // class
    col::GlobalDeclaration *classDeclaration = program.add_declarations();
    llvm2Col::setColNodeId(classDeclaration);
    col::VctClass *vctClass = classDeclaration->mutable_vct_class();
    col::BooleanValue *lockInvariant = vctClass->mutable_intrinsic_lock_invariant()->mutable_boolean_value();
    lockInvariant->set_value(true);
    // class>method
    col::ClassDeclaration *methodDeclaration = vctClass->add_declarations();
    llvm2Col::setColNodeId(methodDeclaration);
    col::InstanceMethod *method = methodDeclaration->mutable_instance_method();
    // class>method>return_type
    method->mutable_return_type()->mutable_t_bool();
    // class>method>body
    col::Block *body = method->mutable_body()->mutable_scope()->mutable_body()->mutable_block();
    col::Return *returnStatement = body->add_statements()->mutable_return_();
    col::BooleanValue *returnValue = returnStatement->mutable_result()->mutable_boolean_value();
    returnValue->set_value(returnBool);
    // class>method>inline
    method->set_inline_(false);
    // class>method>pure
    method->set_pure(false);
    // class>method>contract
    col::ApplicableContract *contract = method->mutable_contract();
    // class>method>contract>precondition
    col::UnitAccountedPredicate *precondition = contract->mutable_requires_()->mutable_unit_accounted_predicate();
    col::BooleanValue *prePred = precondition->mutable_pred()->mutable_boolean_value();
    prePred->set_value(true);
    // class>method>contract>postcondition
    col::UnitAccountedPredicate *postcondition = contract->mutable_ensures()->mutable_unit_accounted_predicate();
    col::Ref *postRefResult = postcondition->mutable_pred()->mutable_result()->mutable_applicable();
    postRefResult->set_index(methodDeclaration->id());
    // class>method>contract>context_everywhere
    col::BooleanValue *contextEverywhere = contract->mutable_context_everywhere()->mutable_boolean_value();
    contextEverywhere->set_value(true);
    return program;
}

static llvm::cl::opt<std::string> inputFileName{"",
                                                llvm::cl::desc{"Module to analyze"},
                                                llvm::cl::value_desc{"IR filename"},
                                                llvm::cl::Positional};
static llvm::cl::opt<bool> testCol{"sample-col",
                                   llvm::cl::desc{"Output a sample col buffer with verdict PASS"}};

static llvm::cl::opt<bool> incorrectTestCol{"sample-col-wrong",
                                            llvm::cl::desc{"Output a sample col buffer with verdict FAIL"}};

int main(int argc, char **argv) {
    llvm::cl::ParseCommandLineOptions(argc, argv);
    // sample mode
    if (testCol.getValue() || incorrectTestCol.getValue()) {
        std::cout << sampleCol(testCol.getValue()).SerializeAsString();
        std::cout.flush();
        return EXIT_SUCCESS;
    }
    // parse mode
    if (inputFileName.empty()) {
        llvm::errs() << "no input file given\n";
        return EXIT_FAILURE;
    }
    llvm::LLVMContext context;
    llvm::SMDiagnostic smDiag;
    auto pModule = parseIRFile(inputFileName, smDiag, context);
    if (!pModule) {
        llvm::errs() << smDiag.getMessage() << '\n';
        return EXIT_FAILURE;
    }
    llvm::Module *module = pModule.release();
    auto pProgram = std::make_shared<col::Program>();
    // Create the analysis managers.
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;
    FAM.registerPass([&] { return llvm::FunctionDeclarer(pProgram); });
    FAM.registerPass([&] { return llvm::FunctionContractDeclarer(pProgram); });
    // Create the new pass manager builder.
    // Take a look at the PassBuilder constructor parameters for more
    // customization, e.g. specifying a TargetMachine or various debugging
    // options.
    llvm::PassBuilder PB;
    // Register all the basic analyses with the managers.
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    llvm::FunctionPassManager FPM;
    FPM.addPass(llvm::FunctionDeclarerPass(pProgram));
    FPM.addPass(llvm::PureAssignerPass(pProgram));
    FPM.addPass(llvm::FunctionContractDeclarerPass(pProgram));
    llvm::ModulePassManager MPM;
    MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
    MPM.run(*module, MAM);
    if (vcllvm::ErrorCollector::hasErrors()) {
        llvm::errs() << "While parsing \"" << inputFileName << "\" VCLLVM has encountered "
                     << vcllvm::ErrorCollector::errorCount() << " error(s):\n";
        for (auto &E: *vcllvm::ErrorCollector::getErrors()) {
            llvm::errs() << E << '\n';
        }
        return EXIT_FAILURE;
    }
    std::cout << pProgram->SerializeAsString();
    return EXIT_SUCCESS;
}
