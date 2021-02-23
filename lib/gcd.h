int gcd(int x, int y) {
    if(x < y) {
        int v = x;
        x = y;
        y = v;
    }
    while(y!=0) {
        int v = x % y;
        x = y;
        y = v;
    }
    return x;
}
