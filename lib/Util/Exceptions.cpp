#include "Util/Exceptions.h"

#include <utility>
#include <sstream>
#include <llvm/Support/raw_ostream.h>

namespace vcllvm {

    std::string UnsupportedTypeException::what() {
        return message;
    }

    UnsupportedTypeException::UnsupportedTypeException(std::string message) {
        this->message = std::move(message);
    }

    u_int32_t ErrorCollector::errorCount;

    void ErrorCollector::addError(const std::string &source, const std::string &message) {
        llvm::errs() << "[" << source << "] " << message << '\n';
        ErrorCollector::errorCount++;
    }

    bool ErrorCollector::hasErrors() {
        return ErrorCollector::errorCount > 0;
    }

    u_int32_t ErrorCollector::getErrorCount() {
        return ErrorCollector::errorCount;
    }
}