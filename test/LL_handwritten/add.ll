define i32 @add(i32 %x, i32 %z, i32 %0, i32 %1) {
  2:
    %3 = add i32 %0, 54
    %test_var = add i32 %x, %z
    %named2 = add i32 %x, %z
    ret i32 %test_var
}