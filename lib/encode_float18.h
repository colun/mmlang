const char * encode_float18$base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void encode_float18(char * & p, const float & f) {
    if(f==0) {
        *p = '@';
        ++p;
        return;
    }
    if(f==1) {
        *p = '!';
        ++p;
        return;
    }
    if(f==-1) {
        *p = '-';
        ++p;
        return;
    }
    if(f==0.5) {
        *p = '$';
        ++p;
        return;
    }
    unsigned int i = *(unsigned int*)&f;
    *p = encode_float18$base64[(i>>26)&63];
    p[1] = encode_float18$base64[(i>>20)&63];
    p[2] = encode_float18$base64[(i>>14)&63];
    p += 3;
}
