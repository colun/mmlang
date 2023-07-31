float decode_float(const char * & p, int float_chars) {
    char c = *p;
    if(c=='@') {
        ++p;
        return 0;
    }
    if(c=='!') {
        ++p;
        return 1;
    }
    if(c=='-') {
        ++p;
        return -1;
    }
    if(c=='$') {
        ++p;
        return 0.5;
    }
    unsigned int i = 0;
    int k = 26;
    for(int j=0; j<float_chars; ++j, k-=6) {
        char c = *p;
        assert('A'<=c && c<='Z' || 'a'<=c && c<='z' || '0'<=c && c<='9' || c=='+' || c=='/');
        if('A'<=c && c<='Z') {
            i |= (unsigned int)(c - 'A') << k;
        }
        else if('a'<=c && c<='z') {
            i |= (unsigned int)(c - ('a' - 26)) << k;
        }
        else if('0'<=c && c<='9') {
            i |= (unsigned int)(c - ('0' - 52)) << k;
        }
        else if(c=='+') {
            i |= 62 << k;
        }
        else if(c=='/') {
            i |= 63 << k;
        }
        else {
            assert(false);
        }
        ++p;
    }
    i |= (1 << (k+5)) - 1;
    return *(float*)&i;
}
