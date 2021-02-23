inline int mod_mul(int x, int y, int p) {
    __asm__ __volatile__("imul %%edx; idiv %2" : "+a"(y), "+d"(x) : "r"(p));
    if(x<0) {
        x += p;
    }
    return x;
}
