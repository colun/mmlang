inline int mod(int x, int p) {
    if(p<=x) {
        do {
            x -= p;
        } while(p<=x);
    }
    else if(x<0) {
        do {
            x += p;
        } while(x<0);
    }
    return x;
}
