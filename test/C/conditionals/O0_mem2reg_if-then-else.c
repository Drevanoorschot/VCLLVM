// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
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