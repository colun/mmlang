inline unsigned int asm_mul_hi(unsigned int x, unsigned int y) {
#if __emscripten__
    return (unsigned int)(((unsigned long long)x * y) >> 32);
#else
    __asm__ __volatile__("mul %%edx" : "+a"(x), "+d"(y));
    return y;
#endif
}
inline unsigned int asm_mul_hi(unsigned int x) {
#if __emscripten__
    return (unsigned int)(((unsigned long long)x * x) >> 32);
#else
    unsigned int y;
    __asm__ __volatile__("mul %%eax" : "+a"(x), "=d"(y));
    return y;
#endif
}
