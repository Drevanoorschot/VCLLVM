// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t
int int_sub(int x, int y) {
    return x - y;
}

long long_sub(long x, long y) {
    return x - y;
}

short short_sub(short x, short y) {
    return x - y;
}