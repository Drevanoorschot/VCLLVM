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
    module->dump();
    return 0;
}