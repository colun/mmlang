tuple<__int128, int> inputFraction128() {
    const char * p = inputCWord();
    bool sign = false;
    if(*p=='-') {
        sign = true;
        ++p;
    }
    __int128 ret = 0;
    int scale = 0;
    bool flag = false;
    while(true) {
        if(*p=='.') {
            ++p;
            flag = true;
        }
        else if('0'<=*p && *p<='9') {
            ret = ret * 10 + (*p - '0');
            ++p;
            if(flag) {
                ++scale;
            }
        }
        else {
            break;
        }
    }
    if(sign) {
        ret = -ret;
    }
    return tuple<__int128, int>(ret, scale);
}
