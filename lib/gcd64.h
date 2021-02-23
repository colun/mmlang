long long gcd(long long x, long long y) {
    if(x < y) {
        long long v = x;
        x = y;
        y = v;
    }
    while(y!=0) {
        long long v = x % y;
        x = y;
        y = v;
    }
    return x;
}
