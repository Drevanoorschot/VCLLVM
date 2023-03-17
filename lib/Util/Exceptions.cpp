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

    u_int32_t ErrorReporter::errorCount;

    void ErrorReporter::addError(const std::string &source, const std::string &message) {
        llvm::errs() << "[" << source << "] " << message << '\n';
        ErrorReporter::errorCount++;
    }

    bool ErrorReporter::hasErrors() {
        return ErrorReporter::errorCount > 0;
    }

    u_int32_t ErrorReporter::getErrorCount() {
        return ErrorReporter::errorCount;
    }
}