


int branch_in_branch(int x, int y) {
    if(x > 0) {
        if(y > 0) {
            return 3;
        } else {
            return 2;
        }
    } else {
        if(y > 0) {
            return 1;
        } else {
            return 0;
        }
    }
}