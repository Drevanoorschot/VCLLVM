// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

int fibonacci(int n) {
    if(n > 2) {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
    return 1;
}

int fibonacci_alt(int n) {
    if(n >= 0 && n <= 1) {
        return n;
    }
    return fibonacci_alt(n - 1) + fibonacci_alt(n - 2);
}

int fib_dyn_help(int n2, int n1, int current, int target) {
    if(current != target) {
        return fib_dyn_help(n1, n2 + n1, current + 1, target);
    }
    return n1;
}

int fib_dyn(int n) {
    return fib_dyn_help(0, 1, 0, n);
}