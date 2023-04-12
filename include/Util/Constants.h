#ifndef VCLLVM_CONSTANTS_H
#define VCLLVM_CONSTANTS_H
#include <string>

namespace vcllvm::constants {
    const std::string VC_PREFIX = "VC.";

    const std::string METADATA_PURE_KEYWORD = VC_PREFIX + "pure";
    const std::string METADATA_CONTRACT_KEYWORD = VC_PREFIX + "contract";
}

#endif //VCLLVM_CONSTANTS_H
