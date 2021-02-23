template<typename Ty, int blockSize = 524288, int align = sizeof(void*)> class fast_pool {
    void * top;
    std::size_t mem;
    std::size_t it;
    std::size_t et;
    std::size_t unitSize;
    int unitSizeNum;
public:
    int count;
    inline void init(std::size_t size=sizeof(Ty)) {
        count = 0;
        while(mem) {
            std::size_t m = *(std::size_t*)mem;
            std::free((void*)mem);
            mem = m;
        }
        top = NULL;
        it = 0;
        et = 0;

        unitSize = ((size+(align-1)) & ~(align-1));
        int unitSizeInt = unitSize;
        int num = (blockSize-(sizeof(void*)+(align-1))) / unitSize;
        unitSizeNum = unitSize * num;
    }
    inline fast_pool() {
        mem = 0;
        init();
    }
    inline void push(Ty * p) {
        --count;
        *(void**)p = top;
        top = (void*)p;
    }
    inline Ty * pop() {
        ++count;
        if(top!=NULL) {
            Ty * result = (Ty*)top;
            top = *(void**)top;
            return result;
        }
        if(it==et) {
            it = (std::size_t)std::malloc(unitSizeNum + (sizeof(void*)+(align-1)));
            *(std::size_t*)it = mem;
            mem = it;
            it = (it+(sizeof(void*)+(align-1))) & ~(align-1);
            et = it + unitSizeNum;
        }
        Ty * result = (Ty*)it;
        it += unitSize;
        return result;
    }
};
