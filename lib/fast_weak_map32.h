template<class T, int N=65536>class fast_weak_map32 {
private:
    const int mask = N - 1;
    fast_array<tuple<unsigned short, T>, N> memo;
public:
    inline void clear() {
        memset(memo.data(), 0, sizeof(memo));
    }
    inline fast_weak_map32() {
        assert((N&(N-1))==0);
    }
    inline bool test(int hash) const {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 1;
        }
        return get<0>(memo[key])==hi;
    }
    inline bool testget(int hash, T & value) const {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 1;
        }
        if(get<0>(memo[key])==hi) {
            value = get<1>(memo[key]);
            return true;
        }
        return false;
    }
    inline void set(int hash, const T & value) {
        unsigned int key = hash & mask;
        unsigned short hi = hash >> 16;
        if(hi==0) {
            hi = 1;
        }
        memo[key] = tuple<unsigned int, T>(hi, value);
    }
    inline const T & get(int hash) const {
        return get<1>(memo[(unsigned int)hash & mask]);
    }
    inline void reset(int hash) {
        unsigned int key = hash & mask;
        get<0>(memo[key]) = 0;
    }
};
