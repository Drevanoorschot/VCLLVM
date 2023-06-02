// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

int foo(int x) {
    return x * x;
}

int bar(int x) {
    return foo(x);
}