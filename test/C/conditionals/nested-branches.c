


int branch_in_branch(int x) {
    int y;
    if(x > 1) {
        if (x % 2 == 0) {
            y = 1;
        } else {
            y = 2;
        }
    } else {
        y = 3;
    }
    return y;
}