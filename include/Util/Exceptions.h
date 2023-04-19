#ifndef VCLLVM_EXCEPTIONS_H
#define VCLLVM_EXCEPTIONS_H

#include <llvm/IR/Module.h>


namespace vcllvm {
    struct UnsupportedTypeException : public std::exception {
        [[nodiscard]] const char *what();
    };

    class ErrorReporter {
    private:
        static u_int32_t errorCount;

        static void addError(const std::string &source, const std::string &message, const std::string &origin);

    public:
        static void addError(const std::string &source, const std::string &message, llvm::Module &llvmModule);

        static void addError(const std::string &source, const std::string &message, llvm::Function &llvmFunction);

        static void addError(const std::string &source, const std::string &message, llvm::BasicBlock &llvmBlock);

        static void addError(const std::string &source, const std::string &message, llvm::Instruction &llvmInstruction);

        static bool hasErrors();

        static u_int32_t getErrorCount();
    };
}
#endif //VCLLVM_EXCEPTIONS_H
