inline void srand49(int seed) {
    mrand49$state = seed + 1442695040888963407ULL;
}
inline void srand49() {
    int clk;
#if __emscripten__
    clk = (int)std::chrono::high_resolution_clock::now().time_since_epoch().count();
#else
    __asm__ __volatile__ ("rdtsc" : "=a"(*(unsigned int*)&clk) : : "%rdx");
#endif
    srand49(clk);
}
