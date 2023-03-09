//
// Created by dre on 3/3/23.
//

#include "Util/Exceptions.h"

#include <utility>

namespace vcllvm {

    std::string UnsupportedTypeException::what() {
        return message;
    }

    UnsupportedTypeException::UnsupportedTypeException(std::string message) {
        this->message = std::move(message);
    }
}