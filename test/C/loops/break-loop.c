

int break_loop(int x) {
    int i = 0;
    while(i < 100) {
        if(i == x) {
            x + x;
            break;
        }
        i++;
    }
    return i;
}