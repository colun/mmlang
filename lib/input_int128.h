__int128 inputInt128() {
    const char * p = inputCWord();
    bool sign = false;
    if(*p=='-') {
        sign = true;
        ++p;
    }
    __int128 ret = 0;
    while('0'<=*p && *p<='9') {
        ret = ret * 10 + (*p - '0');
        ++p;
    }
    return sign ? -ret : ret;
}
