// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

#include "stdint.h"

long long_const() {
    return 42;
}

long long_var(long n) {
    return n;
}

int int_const() {
    return 69;
}

int int_var(int n) {
    return n;
}

short short_const() {
    return 420;
}

short short_var(short n) {
    return n;
}

uint32_t unsigned_const() {
    return 1337;
}

uint32_t unsigned_var(uint32_t n) {
    return n;
}

int64_t signed_const() {
    return -0xDEADBEEF;
}

int64_t signed_var(int64_t n) {
    return n;
}

// unsupported: type mixing: short -> long