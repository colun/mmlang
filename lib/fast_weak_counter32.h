template<int B=16>class fast_weak_counter32 {
private:
    static const int N = 1 << B;
    static const int mask = N - 1;
    fast_array<tuple<unsigned short, unsigned short>, N> memo;
public:
    inline void clear() {
        memset(memo.data(), 0, sizeof(memo));
    }
    inline fast_weak_counter32() {
        assert(1<=B);
        assert(B<=24);
    }
    inline int count(int hash) {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 0x8000;
        }
        if(std::get<0>(memo[key])!=hi) {
            memo[key] = tuple<unsigned short, unsigned short>(hi, 0);
        }
        return ++std::get<1>(memo[key]);
    }
    inline int get(int hash) const {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 0x8000;
        }
        if(std::get<0>(memo[key])!=hi) {
            return 0;
        }
        return std::get<1>(memo[key]);
    }
    inline void reset(int hash) {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 0x8000;
        }
        if(std::get<0>(memo[key])==hi) {
            std::get<1>(memo[key]) = 0;
        }
    }
};
