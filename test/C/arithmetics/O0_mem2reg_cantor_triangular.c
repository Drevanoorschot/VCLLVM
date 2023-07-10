// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

int nth_triangular(int n) {
    return (n * (n + 1)) / 2;
}

int cantor_pair(int x, int y) {
    return ((x * x) + x + (2 * x * y) + (3 * y) + (y * y)) / 2;
}