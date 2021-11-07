inline void getCpuClock(unsigned long long & t) {
    __asm__ __volatile__ ("rdtsc" : "=a"(*(unsigned int*)&t), "=d"(((unsigned int*)&t)[1]));
}
inline unsigned long long getCpuClock() {
    unsigned long long t;
    getCpuClock(t);
    return t;
}
