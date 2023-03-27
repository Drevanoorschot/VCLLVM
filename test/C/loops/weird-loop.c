
int weird_loop(int x) {
    int i = 0;
    while(1) {
        if (i == x) {
            break;
        }
        i++;
    }
    return i;
}