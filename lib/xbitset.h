template<int N, bool use_xmem=true> class xbitset {
private:
    static const int qword_size = (N+63)>>6;
public:
    unsigned long long data_[qword_size];
    inline xbitset() noexcept {
        if(use_xmem) {
            xmem::init(this);
        }
        memset(data_, 0, sizeof(data_));
    }
    template<bool use_xmem2> inline xbitset(const xbitset<N, use_xmem2> & o) noexcept {
        if(use_xmem) {
            xmem::init(this);
        }
        memcpy(data_, o.data_, sizeof(data_));
    }
    template<bool use_xmem2> inline void operator = (const xbitset<N, use_xmem2> & o) noexcept {
        if(use_xmem) {
            int start = -1000;
            int last = -1001;
            for(int i=0; i<qword_size; ++i) {
                if(data_[i]!=o.data_[i]) {
                    if(last+1!=i) {
                        if(0<=start) {
                            xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                            for(int j=start; j<=last; ++j) {
                                data_[j] = o.data_[j];
                            }
                        }
                        last = start = i;
                    }
                    else {
                        last = i;
                    }
                }
            }
            if(0<=start) {
                xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                for(int j=start; j<=last; ++j) {
                    data_[j] = o.data_[j];
                }
            }
        }
        else {
            memcpy(data_, o.data_, sizeof(data_));
        }
    }
    template<bool use_xmem2> inline void operator &= (const xbitset<N, use_xmem2> & o) noexcept {
        if(use_xmem) {
            int start = -1000;
            int last = -1001;
            for(int i=0; i<qword_size; ++i) {
                if((data_[i]&~o.data_[i])!=0) {
                    if(last+1!=i) {
                        if(0<=start) {
                            xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                            for(int j=start; j<=last; ++j) {
                                data_[j] &= o.data_[j];
                            }
                        }
                        last = start = i;
                    }
                    else {
                        last = i;
                    }
                }
            }
            if(0<=start) {
                xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                for(int j=start; j<=last; ++j) {
                    data_[j] &= o.data_[j];
                }
            }
        }
        else {
            for(int i=0; i<qword_size; ++i) {
                data_[i] &= o.data_[i];
            }
        }
    }
    template<bool use_xmem2> inline void operator |= (const xbitset<N, use_xmem2> & o) noexcept {
        if(use_xmem) {
            int start = -1000;
            int last = -1001;
            for(int i=0; i<qword_size; ++i) {
                if((o.data_[i]&~data_[i])!=0) {
                    if(last+1!=i) {
                        if(0<=start) {
                            xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                            for(int j=start; j<=last; ++j) {
                                data_[j] |= o.data_[j];
                            }
                        }
                        last = start = i;
                    }
                    else {
                        last = i;
                    }
                }
            }
            if(0<=start) {
                xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                for(int j=start; j<=last; ++j) {
                    data_[j] |= o.data_[j];
                }
            }
        }
        else {
            for(int i=0; i<qword_size; ++i) {
                data_[i] |= o.data_[i];
            }
        }
    }
    template<bool use_xmem2> inline void operator ^= (const xbitset<N, use_xmem2> & o) noexcept {
        if(use_xmem) {
            int start = -1000;
            int last = -1001;
            for(int i=0; i<qword_size; ++i) {
                if(o.data_[i]!=0) {
                    if(last+1!=i) {
                        if(0<=start) {
                            xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                            for(int j=start; j<=last; ++j) {
                                data_[j] ^= o.data_[j];
                            }
                        }
                        last = start = i;
                    }
                    else {
                        last = i;
                    }
                }
            }
            if(0<=start) {
                xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                for(int j=start; j<=last; ++j) {
                    data_[j] ^= o.data_[j];
                }
            }
        }
        else {
            for(int i=0; i<qword_size; ++i) {
                data_[i] ^= o.data_[i];
            }
        }
    }
    inline void set() noexcept {
        if(use_xmem) {
            int start = -1000;
            int last = -1001;
            for(int i=0; i<qword_size-1; ++i) {
                if(data_[i]!=-1ULL) {
                    if(last+1!=i) {
                        if(0<=start) {
                            xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                            memset(data_+start, -1, sizeof(data_[0])*(last+1-start));
                        }
                        last = start = i;
                    }
                    else {
                        last = i;
                    }
                }
            }
            if(data_[qword_size-1]!=(-1ULL >> ((qword_size<<6)-N))) {
                if(last+1!=qword_size-1) {
                    if(0<=start) {
                        xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                        memset(data_+start, -1, sizeof(data_[0])*(last+1-start));
                    }
                    last = start = qword_size-1;
                }
                else {
                    last = qword_size-1;
                }
                xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                if(start!=last) {
                    memset(data_+start, -1, sizeof(data_[0])*(last-start));
                }
                data_[qword_size-1] = -1ULL >> ((qword_size<<6)-N);
                return;
            }
            if(0<=start) {
                xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                memset(data_+start, -1, sizeof(data_[0])*(last+1-start));
            }
        }
        else {
            for(int i=0; i<qword_size-1; ++i) {
                data_[i] = -1ULL;
            }
            data_[qword_size-1] = -1ULL >> ((qword_size<<6)-N);
        }
    }
    inline void reset() noexcept {
        if(use_xmem) {
            int start = -1000;
            int last = -1001;
            for(int i=0; i<qword_size; ++i) {
                if(data_[i]!=0) {
                    if(last+1!=i) {
                        if(0<=start) {
                            xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                            memset(data_+start, 0, sizeof(data_[0])*(last+1-start));
                        }
                        last = start = i;
                    }
                    else {
                        last = i;
                    }
                }
            }
            if(0<=start) {
                xmem::modify(data_+start, sizeof(data_[0])*(last+1-start));
                memset(data_+start, 0, sizeof(data_[0])*(last+1-start));
            }
        }
        else {
            memset(data_, 0, sizeof(data_));
        }
    }
    inline void flip() noexcept {
        xmem::modify(&data_, (N+7)>>3);
        for(int i=0; i<qword_size-1; ++i) {
            data_[i] ^= -1ULL;
        }
        data_[qword_size-1] ^= -1ULL >> ((qword_size<<5)-N);
    }
    inline bool test(int i) const noexcept {
        return (((int*)data_)[i>>5] & (1<<(i&31)))!=0;
    }
    inline void set(int i) noexcept {
        if(use_xmem) {
            if((((int*)data_)[i>>5] & (1<<(i&31)))==0) {
                xmem::modify((char*)data_ + (i>>3), sizeof(char));
                ((int*)data_)[i>>5] |= (1<<(i&31));
            }
        }
        else {
            ((int*)data_)[i>>5] |= (1<<(i&31));
        }
    }
    inline void reset(int i) noexcept {
        if(use_xmem) {
            if((((int*)data_)[i>>5] & (1<<(i&31)))!=0) {
                xmem::modify((char*)data_ + (i>>3), sizeof(char));
                ((int*)data_)[i>>5] &= ~(1<<(i&31));
            }
        }
        else {
            ((int*)data_)[i>>5] &= ~(1<<(i&31));
        }
    }
    inline void flip(int i) noexcept {
        if(use_xmem) {
            xmem::modify((char*)data_ + (i>>3), sizeof(char));
        }
        ((int*)data_)[i>>5] ^= (1<<(i&31));
    }
    inline int count() const noexcept {
        int ret = 0;
        for(int i=0; i<qword_size; ++i) {
            unsigned long long n = data_[i];
            n = (n&0x5555555555555555ULL) + ((n>>1)&0x5555555555555555ULL);// 1/1->1/3->2/3
            n = (n&0x3333333333333333ULL) + ((n>>2)&0x3333333333333333ULL);// 2/3->2/15->4/15
            n = (n+(n>>4))&0x0F0F0F0F0F0F0F0FULL;// 4/15->8/15->8/255
            n = n+(n>>8);
            n = n+(n>>16);
            ret += (int)(n+(n>>32)) & 255;
        }
        return ret;
    }
};
