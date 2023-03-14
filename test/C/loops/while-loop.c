// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t
int while_loop_add(int x, int y) {
    int i = 0;
    while (i != y) {
        x++;
        i++;
    }
    return x;
}