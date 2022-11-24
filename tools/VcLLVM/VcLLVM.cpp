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
#include "Passes/EntryBlockParser.h"

using namespace llvm;

static cl::opt<std::string> inputFileName{cl::Positional,
                                          cl::desc{"<Module to analyze>"},
                                          cl::value_desc{"IR filename"},
                                          cl::init(""),
                                          cl::Required};

int main(int argc, char **argv) {
    cl::ParseCommandLineOptions(argc, argv);

    LLVMContext context;
    SMDiagnostic smDiag;
    auto pModule = parseIRFile(inputFileName,
                               smDiag, context);
    if (!pModule) {
        errs() << smDiag.getMessage() << "\n";
        return 1;
    }
    Module *module = pModule.get();
    auto pAst = std::make_shared<AST::Program>();
    // Create the analysis managers.
    LoopAnalysisManager lam;
    FunctionAnalysisManager fam;
    CGSCCAnalysisManager cgam;
    ModuleAnalysisManager mam;
    // Create the new pass manager builder.
    // Take a look at the PassBuilder constructor parameters for more
    // customization, e.g. specifying a TargetMachine or various debugging
    // options.
    PassBuilder pb;

    // Register all the basic analyses with the managers.
    pb.registerModuleAnalyses(mam);
    pb.registerCGSCCAnalyses(cgam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);
    pb.crossRegisterProxies(lam, fam, cgam, mam);

    FunctionPassManager fpm;
    fpm.addPass(VariableNamer(pAst));
    fpm.addPass(FunctionDeclarer(pAst));
    fpm.addPass(EntryBlockParser(pAst));
    ModulePassManager mpm;
    mpm.addPass(createModuleToFunctionPassAdaptor(std::move(fpm)));

    // Optimize the IR!
    mpm.run(*module, mam);
    pAst->print(std::cout);
    return EXIT_SUCCESS;
}