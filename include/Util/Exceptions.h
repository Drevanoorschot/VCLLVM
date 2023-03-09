//
// Created by dre on 3/3/23.
//

#ifndef VCLLVM_EXCEPTIONS_H
#define VCLLVM_EXCEPTIONS_H

#include <stdexcept>

namespace vcllvm {
    class UnsupportedTypeException : public std::exception {
    private:
        std::string message;
    public:
        explicit UnsupportedTypeException(std::string message);
        std::string what();
    };
}
#endif //VCLLVM_EXCEPTIONS_H
