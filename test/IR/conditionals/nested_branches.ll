define i32 @branch_in_branch(i32  %0, i32  %1) {
  %3 = icmp sgt i32 %0, 0
  br i1 %3, label %4, label %8

4:                                                ; preds = %2
  %5 = icmp sgt i32 %1, 0
  br i1 %5, label %6, label %7

6:                                                ; preds = %4
  br label %12

7:                                                ; preds = %4
  br label %12

8:                                                ; preds = %2
  %9 = icmp sgt i32 %1, 0
  br i1 %9, label %10, label %11

10:                                               ; preds = %8
  br label %12

11:                                               ; preds = %8
  br label %12

12:                                               ; preds = %11, %10, %7, %6
  %.0 = phi i32 [ 3, %6 ], [ 2, %7 ], [ 1, %10 ], [ 0, %11 ]
  ret i32 %.0
}
