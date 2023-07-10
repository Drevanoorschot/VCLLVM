// RUN: clang -S -Xclang -disable-O0-optnone -emit-llvm %s -o %t
// RUN: opt-15 -S -mem2reg %t -o %t
// RUN: %VCLLVM %t

#include "stdbool.h"

bool is_later(
        int y1, int m1, int d1,
        int y2, int m2, int d2)
{
    if (y1 != y2) {
        return y1 > y2;
    } else if (m1 != m2) {
        return m1 > m2;
    } else {
        return d1 > d2;
    }
}