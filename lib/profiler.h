int MM$P$nowLine = -1;
struct MM$Profiler {
    std::thread th;
    bool continue_flag;
    int total_count;
    std::map<int, int> counts;
    MM$Profiler() {
        fprintf(stderr, "Start, profiler!\n");
        continue_flag = true;
        th = std::thread([&] {
            total_count = 0;
            unsigned long long reserve;
            unsigned long long t;
            constexpr int nextClock = 10000;
            getCpuClock(reserve);
            reserve += nextClock;
            while(continue_flag) {
                getCpuClock(t);
                if(reserve<=t) {
                    reserve += nextClock;
                    ++total_count;
                    if(0<=MM$P$nowLine) {
                        ++counts[MM$P$nowLine];
                    }
                }
            }
        });
    }
    ~MM$Profiler() {
        int other = total_count;
        continue_flag = false;
        th.join();
        fprintf(stderr, "Profiler results:\n");
        for(auto & v : counts) {
            double p = (100.0 * get<1>(v)) / total_count;
            if(0.01<=p) {
                other -= get<1>(v);
                fprintf(stderr, "%5d: %7.3f%%\n", get<0>(v), p);
            }
        }
        fprintf(stderr, "Other: %7.3f%%\n", (100.0 * other) / total_count);
    }
};
MM$Profiler MM$profiler;
inline void MM$P(int line) {
    MM$P$nowLine = line;
}
struct MM$P$Func {
    int line;
    inline MM$P$Func() {
        line = MM$P$nowLine;
    }
    inline ~MM$P$Func() {
        MM$P$nowLine = line;
    }
};
