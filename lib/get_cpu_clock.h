inline void getCpuClock(unsigned long long & t) {
#if __emscripten__
    t = std::chrono::high_resolution_clock::now().time_since_epoch().count();
#else
    __asm__ __volatile__ ("rdtsc" : "=a"(*(unsigned int*)&t), "=d"(((unsigned int*)&t)[1]));
#endif
}
inline unsigned long long getCpuClock() {
    unsigned long long t;
    getCpuClock(t);
    return t;
}
