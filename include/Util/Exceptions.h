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
         static std::vector<std::string> errors;
    public:
        static void addError(const std::string &source, const std::string &message);

        static std::vector<std::string> *getErrors();

        static bool hasErrors();

        static u_int32_t errorCount();
    };
}
#endif //VCLLVM_EXCEPTIONS_H
