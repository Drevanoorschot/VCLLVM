// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t
int add(int x, int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

int multiply(int x, int y) {
    return x * y;
}

int divide(int x, int y) {
    return x / y;
}
