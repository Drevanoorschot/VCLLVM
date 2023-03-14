#include "Util/Exceptions.h"

#include <utility>
#include <sstream>

namespace vcllvm {

    std::string UnsupportedTypeException::what() {
        return message;
    }

    UnsupportedTypeException::UnsupportedTypeException(std::string message) {
        this->message = std::move(message);
    }

    std::vector<std::string> ErrorCollector::errors;

    void ErrorCollector::addError(const std::string &source, const std::string &message) {
        std::stringstream stream;
        stream << "[" << source << "] " << message << '\n';
        ErrorCollector::errors.push_back(stream.str());
    }

    std::vector<std::string> *ErrorCollector::getErrors() {
        return &ErrorCollector::errors;
    }

    bool ErrorCollector::hasErrors() {
        return !ErrorCollector::errors.empty();
    }

    u_int32_t ErrorCollector::errorCount() {
        return ErrorCollector::errors.size();
    }
}