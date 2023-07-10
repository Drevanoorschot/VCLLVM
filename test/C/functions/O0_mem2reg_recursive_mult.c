// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

int recursive_mult(int x, int y) {
    if (x > 0) {
        return y + recursive_mult(x - 1, y);
    }
    return 0;
}