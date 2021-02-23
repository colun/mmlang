inline long long mmrand49() {
    int v = mrand49();
    return ((long long)v << 32) | mrand49();
}
