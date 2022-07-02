struct xmem {
    static char * base_ptr;
    static int buffer_size;
    static int redo_size;
    static std::vector<tuple<int, int> > build_work;
    static std::vector<tuple<int, int> > locks;
    static char buffer[262144];
    std::vector<char*> mems;
    static constexpr int max_memsize = 1048576 - 256;
    int used_mems;
    int mymemsize = 0;
    char * mymem = 0;
    inline void clear() {
        used_mems = 0;
        mymemsize = 0;
    }
    inline xmem() {
        clear();
    }
    inline ~xmem() {
        //for(char*m : mems) {
        //    std::free((void*)m);
        //}
    }
    inline char * alloc(int size) {
        if(mymemsize<size) {
            assert(size<=max_memsize);
            if(mems.size()<=used_mems) {
                mems.push_back((char*)std::malloc(max_memsize));
            }
            mymem = mems[used_mems++];
            mymemsize = max_memsize;
        }
        char * ret = mymem;
        mymem += size;
        mymemsize -= size;
        return ret;
    }
    static inline void free(void * patch) {
        // Never free!
    }
    static inline void lock() {
        locks.emplace_back(buffer_size, redo_size);
        redo_size = 0;
    }
    static inline void unlock() {
        assert(!locks.empty());
        std::tie(buffer_size, redo_size) = locks.back();
        locks.pop_back();
    }
    static inline bool enabled() {
        return !locks.empty();
    }
    static inline void init(void * mem) {
        if unlikely(!base_ptr) {
            base_ptr = (char*)mem;
        }
    }
    static inline void modify(void * mem, int size) {
        if(locks.empty()) {
            return;
        }
        assert(1<=size);
        assert(size<65536);
        assert(buffer_size+size+6<=sizeof(buffer));
        std::memcpy(buffer+buffer_size, mem, size);
        buffer_size += size;
        *(unsigned short*)(buffer+buffer_size) = size;
        buffer_size += 2;
        *(int*)(buffer+buffer_size) = ((char*)mem)-base_ptr;
        buffer_size += 4;
        redo_size += size;
    }
    static inline void undo() {
        assert(!locks.empty());
        int base_buffer_size = std::get<0>(locks.back());
        while(base_buffer_size<buffer_size) {
            buffer_size -= 4;
            char * p = base_ptr + *(int*)(buffer+buffer_size);
            buffer_size -= 2;
            int size = *(unsigned short*)(buffer+buffer_size);
            buffer_size -= size;
            std::memcpy(p, buffer+buffer_size, size);
        }
        assert(base_buffer_size==buffer_size);
        redo_size = 0;
    }
    static inline void undo(void * patch) {
        assert(patch);
        char * p = (char*)patch;
        int sz = *(unsigned int*)p;
        char * ep = p + sz;
        p += 4;
        while(p<ep) {
            int pos = *(int*)p;
            p += 4;
            assert(p<=ep);
            int size = *(unsigned short*)p;
            p += 2 + size;
            assert(p+size<=ep);
            std::memcpy(base_ptr+pos, p, size);
            p += size;
        }
        assert(p==ep);
    }
    inline void * build_undo() {
        //*
        assert(!locks.empty());
        int base_buffer_size = std::get<0>(locks.back());
        int bs = buffer_size;
        assert(build_work.empty());
        while(base_buffer_size<bs) {
            bs -= 4;
            int pos = *(int*)(buffer+bs);
            bs -= 2;
            int size = *(unsigned short*)(buffer+bs);
            bs -= size;
            build_work.emplace_back(pos, size);
        }
        int total_size = 0;
        if(!build_work.empty()) {
            std::sort(build_work.begin(), build_work.end());
            {
                int j=0;
                int block_pos = get<0>(build_work[0]);
                int block_end = block_pos + get<1>(build_work[0]);
                for(int i=1; i<build_work.size(); ++i) {
                    int next_block_pos = get<0>(build_work[i]);
                    int next_block_end = next_block_pos + get<1>(build_work[i]);
                    if(next_block_pos<=block_end+16) {
                        block_end = max(block_end, next_block_end);
                    }
                    else {
                        int block_size = block_end - block_pos;
                        assert(block_size<65536);
                        get<0>(build_work[j]) = block_pos;
                        get<1>(build_work[j]) = block_size;
                        total_size += block_size;
                        ++j;
                        block_pos = next_block_pos;
                        block_end = next_block_end;
                    }
                }
                {
                    int block_size = block_end - block_pos;
                    assert(block_size<65536);
                    get<0>(build_work[j]) = block_pos;
                    get<1>(build_work[j]) = block_size;
                    total_size += block_size;
                }
                build_work.resize(j+1);
            }
        }
        int sz = (int)build_work.size() * 6 + total_size + total_size + 4;
        char * ret = alloc(sz);
        *(unsigned int*)ret = sz;
        char * p = ret + 4;
        for(int i=0; i<build_work.size(); ++i) {
            int pos = get<0>(build_work[i]);
            int size = get<1>(build_work[i]);
            *(int*)p = pos;
            p += 4;
            *(unsigned short*)p = size;
            p += 2;
            std::memcpy(p, base_ptr+pos, size);
            p += size + size;
        }
        undo();
        p = ret + 4;
        for(int i=0; i<build_work.size(); ++i) {
            int pos = get<0>(build_work[i]);
            int size = get<1>(build_work[i]);
            p += 6 + size;
            std::memcpy(p, base_ptr+pos, size);
            p += size;
        }
        assert(p==ret+sz);
        build_work.clear();
        return ret;
    }
    inline void * build() {
        void * ret = build_undo();
        redo(ret);
        return ret;
    }
    static inline void redo(void * patch) {
        //UNDO時の順番とREDO時の順番は逆だが、REDOデータはbuild時に作られるため、どの順番で実行されても問題ない。
        assert(patch);
        char * p = (char*)patch;
        int sz = *(unsigned int*)p;
        char * ep = p + sz;
        p += 4;
        while(p!=ep) {
            int pos = *(int*)p;
            p += 4;
            assert(p<=ep);
            int size = *(unsigned short*)p;
            p += 2;
            assert(p+size+size<=ep);
            std::memcpy(base_ptr+pos, p, size);
            p += size + size;
        }
        assert(p==ep);
    }
};
char * xmem::base_ptr = 0;
int xmem::buffer_size = 0;
int xmem::redo_size = 0;
std::vector<std::tuple<int, int> > xmem::locks;
std::vector<std::tuple<int, int> > xmem::build_work;
char xmem::buffer[262144];

inline void xmem_lock() {
    xmem::lock();
}
inline void xmem_unlock() {
    xmem::unlock();
}
inline void xmem_undo() {
    xmem::undo();
}
