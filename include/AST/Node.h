#ifndef VCLLVM_NODE_H
#define VCLLVM_NODE_H

#include <string>
#include <llvm/IR/Value.h>

#include "col.pb.h"

namespace ast {
    namespace col = vct::col::serialize;

    template<class colT>
    class Node {
    protected:
        std::string origin;
        const llvm::Value &llvmRelatedValue;
        colT &mutableColNode;

        void setOrigin(std::string newOrigin) {
            this->origin = newOrigin;
            mutableColNode.set_origin(newOrigin);
        }

        explicit Node(colT &mutableColNode, llvm::Value &llvmValue) :
                mutableColNode(mutableColNode), llvmRelatedValue(llvmValue) {}
    public:
        colT &getMutableColNode() {
            return mutableColNode;
        }
    };

    class FunctionNode : public Node<col::LlvmFunctionDefinition> {

    };

    class ProgramNode : public Node<col::Program> {
    private:
        std::vector<FunctionNode> functionNodes;
    public:
        explicit ProgramNode(col::Program &colProgram, llvm::Module &llvmModule);

        FunctionNode &getFunctionNode(int i);

        void addFunctionNode(FunctionNode &functionNode);
    };
}
#endif //VCLLVM_NODE_H
