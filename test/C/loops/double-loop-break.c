

int double_break(int x) {
    for(int i = 0; i < x; i++) {
        for(int j = x; j > 0; j--) {
            if(j == i) {
                goto lbl;
            }
        }
    }
    lbl:
    return x;
}