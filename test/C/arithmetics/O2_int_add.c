// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t
int int_add(int x, int y) {
    return x + y;
}

long long_add(long x, long y) {
    return x + y;
}

short short_add(short x, short y) {
    return x + y;
}