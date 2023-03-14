// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t
int for_loop_add(int x, int y) {
    for (int i = 0; i < y; i++) {
        x++;
    }
    return x;
}