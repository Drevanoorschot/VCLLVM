// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t
int int_mul(int x, int y) {
    return x * y;
}

long long_mul(long x, long y) {
    return x * y;
}

short short_mul(short x, short y) {
    return x * y;
}