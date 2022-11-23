#include <llvm/Support/CommandLine.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <iostream>
#include <memory>
#include "AST/AST.h"
#include "Passes/FunctionDeclarer.h"
#include "Passes/VariableNamer.h"

using namespace llvm;

static cl::opt<std::string> InputFileName{cl::Positional,
                                          cl::desc{"<Module to analyze>"},
                                          cl::value_desc{"IR filename"},
                                          cl::init(""),
                                          cl::Required};

int main(int argc, char **argv) {
    cl::ParseCommandLineOptions(argc, argv);

    LLVMContext Ctx;
    SMDiagnostic SMDiag;
    auto Mptr = parseIRFile(InputFileName,
                            SMDiag, Ctx);
    if (!Mptr) {
        errs() << SMDiag.getMessage() << "\n";
        return 1;
    }
    Module *module = Mptr.get();
    auto p_AST = std::make_shared<AST::Program>();
    // Create the analysis managers.
    LoopAnalysisManager LAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;
    ModuleAnalysisManager MAM;
    // Create the new pass manager builder.
    // Take a look at the PassBuilder constructor parameters for more
    // customization, e.g. specifying a TargetMachine or various debugging
    // options.
    PassBuilder PB;

    // Register all the basic analyses with the managers.
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    FunctionPassManager FPM;
    FPM.addPass(VariableNamer(p_AST));
    FPM.addPass(FunctionDeclarer(p_AST));
    ModulePassManager MPM;
    MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));

    // Optimize the IR!
    MPM.run(*module, MAM);
    p_AST->print(std::cout);
    return EXIT_SUCCESS;
}