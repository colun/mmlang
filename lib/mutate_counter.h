class mutate_counter {
#ifndef NDEBUG
    unsigned int total_count;
    unsigned int accept_counts[5];
#endif
public:
    mutate_counter() {
#ifndef NDEBUG
        total_count = 0;
        for(int i=0; i<5; ++i) {
            accept_counts[i] = 0;
        }
#endif
    }
    void accept() {
#ifndef NDEBUG
        ++total_count;
        ++accept_counts[4];
        if((total_count&(total_count-1))==0) {
            accept_counts[0] += accept_counts[1];
            for(int i=1; i<4; ++i) {
                accept_counts[i] = accept_counts[i+1];
            }
            accept_counts[4] = 0;
        }
#endif
    }
    void reject() {
#ifndef NDEBUG
        ++total_count;
        if((total_count&(total_count-1))==0) {
            accept_counts[0] += accept_counts[1];
            for(int i=1; i<4; ++i) {
                accept_counts[i] = accept_counts[i+1];
            }
            accept_counts[4] = 0;
        }
#endif
    }
    void report(const char * label) {
#ifndef NDEBUG
        if(!total_count) {
            return;
        }
        unsigned int msb = total_count;
        msb |= msb >> 1;
        msb |= msb >> 2;
        msb |= msb >> 4;
        msb |= msb >> 8;
        msb |= msb >> 16;
        msb = (msb+1) >> 1;
        fprintf(stderr, "%s: %u %.2f%% %.2f%% %.2f%% %.2f%%\n", label, total_count,
            100.0 * accept_counts[0] / max(1u, msb>>3),
            100.0 * accept_counts[1] / max(1u, msb>>3),
            100.0 * accept_counts[2] / max(1u, msb>>2),
            100.0 * (accept_counts[3]+accept_counts[4]) / max(1u, total_count-(msb>>1)));
#endif
    }
};
