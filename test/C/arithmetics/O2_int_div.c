// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t

#include "stdint.h"

uint32_t unsignedDiv(uint32_t x, uint32_t y) {
    return x / y;
}


int32_t signedDiv(int32_t x, int32_t y) {
    return x / y;
}

int32_t signedByUnsigned(int32_t x, uint32_t y) {
    return x / y;
}

int32_t unsignedBySigned(uint32_t x, int32_t y) {
    return x / y;
}