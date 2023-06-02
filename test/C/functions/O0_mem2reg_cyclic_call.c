// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

int bar(int x);

int foo(int x) {
    if(x <= 0) {
        return 1;
    }
    return bar(x - 1);
}

int bar(int x) {
    if(x <= 0) {
        return 0;
    }
    return foo(x - 1);
}