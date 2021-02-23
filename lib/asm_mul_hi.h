inline unsigned int asm_mul_hi(unsigned int x, unsigned int y) {
    __asm__ __volatile__("mul %%edx" : "+a"(x), "+d"(y));
    return y;
}
inline unsigned int asm_mul_hi(unsigned int x) {
    unsigned int y;
    __asm__ __volatile__("mul %%eax" : "+a"(x), "=d"(y));
    return y;
}
