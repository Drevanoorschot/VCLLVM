// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

#include "stdbool.h"


bool const_true() {
    return true;
}

bool const_false() {
    return false;
}

// unsupported: variable boolean return (causes a truncate on return)