// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

int factorial(int x) {
    if(x <= 0) {
        return 1;
    }
    return x * factorial(x - 1);
}