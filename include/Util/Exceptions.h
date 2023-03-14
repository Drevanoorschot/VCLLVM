#ifndef VCLLVM_EXCEPTIONS_H
#define VCLLVM_EXCEPTIONS_H

#include <stdexcept>
#include <vector>

namespace vcllvm {
    class UnsupportedTypeException : public std::exception {
    private:
        std::string message;
    public:
        explicit UnsupportedTypeException(std::string message);

        std::string what();
    };

    class ErrorCollector {
    private:
         static u_int32_t errorCount;
    public:
        static void addError(const std::string &source, const std::string &message);

        static bool hasErrors();

        static u_int32_t getErrorCount();
    };
}
#endif //VCLLVM_EXCEPTIONS_H
