

int evenSum(int x) {
    int res = 0;
    for(int i = 0; i < x; i++) {
        if(i % 2 == 0) {
            res += i;
        }
    }
    return res;
}