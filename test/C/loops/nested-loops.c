

int for_for_loop_mult(int x, int y) {
    int res = 0;
    for(int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            res++;
        }
    }
    return res;
}

int while_while_loop_mult(int x, int y) {
    int res = 0;
    int i = 0;
    while(i < x) {
        int j = 0;
        while(j < y) {
            res++;
            j++;
        }
        i++;
    }
    return res;
}

int for_while_loop_mult(int x, int y) {
    int res = 0;
    for (int i = 0; i < x; i++) {
        int j = 0;
        while (j < y) {
            res++;
            j++;
        }
    }
    return res;
}

int while_for_loop_mult(int x, int y) {
    int res = 0;
    int i = 0;
    while(i < x) {
        for (int j = 0; j < y; j++) {
            res++;
        }
        i++;
    }
    return res;
}