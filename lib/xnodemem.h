constexpr int xnodemem$memsize = 65536 - 256;
std::vector<char*> xnodemem$unused;
struct xnodemem {
    char * p = 0;
    int size = 0;
    std::vector<char*> used;
    inline void swap(xnodemem & o) {
        std::swap(p, o.p);
        std::swap(size, o.size);
        used.swap(o.used);
    }
    inline char * alloc(int min_size=4096) {
        if(size<min_size) {
            if(xnodemem$unused.empty()) {
                p = (char*)std::malloc(xnodemem$memsize);
            }
            else {
                p = xnodemem$unused.back();
                xnodemem$unused.pop_back();
            }
            used.emplace_back(p);
            size = xnodemem$memsize;
        }
        return p;
    }
    inline char * now() {
        return p;
    }
    inline void skip(int sz) {
        size -= sz;
        assert(0<=size);
        p += sz;
    }
    template<class T> inline void next(const T & value) {
        size -= sizeof(T);
        assert(0<=size);
        *(T*)p = value;
        p += sizeof(T);
    }
    inline void free() {
        for(char * p : used) {
            xnodemem$unused.emplace_back(p);
        }
        used.clear();
        size = 0;
    }
};
