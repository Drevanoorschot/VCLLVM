// RUN: clang -S -O2 -emit-llvm %s -o %t
// RUN: %VCLLVM %t
int if_then_else(int cond) {
    int i;
    if(cond) {
        i = 1;
    } else {
        i = 2;
    }
    return i;
}