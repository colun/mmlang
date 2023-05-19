float decode_float18(char * & p) {
    if(*p=='@') {
        ++p;
        return 0;
    }
    if(*p=='!') {
        ++p;
        return 1;
    }
    if(*p=='-') {
        ++p;
        return -1;
    }
    if(*p=='$') {
        ++p;
        return 0.5;
    }
    unsigned int i = (1 << 13) - 1;
    char c = *p;
    assert('A'<=c && c<='Z' || 'a'<=c && c<='z' || '0'<=c && c<='9');
    if('A'<=c && c<='Z') {
        i += (unsigned int)(c - 'A') << 26;
    }
    else if('a'<=c && c<='z') {
        i += (unsigned int)(c - ('a' - 26)) << 26;
    }
    else if('0'<=c && c<='9') {
        i += (unsigned int)(c - ('0' - 52)) << 26;
    }
    c = p[1];
    assert('A'<=c && c<='Z' || 'a'<=c && c<='z' || '0'<=c && c<='9');
    if('A'<=c && c<='Z') {
        i += (unsigned int)(c - 'A') << 20;
    }
    else if('a'<=c && c<='z') {
        i += (unsigned int)(c - ('a' - 26)) << 20;
    }
    else if('0'<=c && c<='9') {
        i += (unsigned int)(c - ('0' - 52)) << 20;
    }
    c = p[2];
    assert('A'<=c && c<='Z' || 'a'<=c && c<='z' || '0'<=c && c<='9');
    if('A'<=c && c<='Z') {
        i += (unsigned int)(c - 'A') << 14;
    }
    else if('a'<=c && c<='z') {
        i += (unsigned int)(c - ('a' - 26)) << 14;
    }
    else if('0'<=c && c<='9') {
        i += (unsigned int)(c - ('0' - 52)) << 14;
    }
    p += 3;
    return *(float*)&i;
}
