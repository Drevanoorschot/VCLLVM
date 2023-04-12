#include "AST/Node.h"

#include "Transform/Origin/OriginProvider.h"

namespace ast {

    ProgramNode::ProgramNode(col::Program &colProgram, llvm::Module &llvmModule) :
            Node(colProgram, *llvm::cast<llvm::Value>(&llvmModule)) {
        setOrigin(llvm2Col::generateProgramOrigin(llvmModule));
    }

    FunctionNode &ProgramNode::getFunctionNode(int i) {
        return functionNodes.at(i);
    }

    void ProgramNode::addFunctionNode(FunctionNode &functionNode) {
        functionNodes.push_back(functionNode);
    }
}