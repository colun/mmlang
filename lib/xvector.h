template<typename T, int N, bool use_xmem=true> class xvector {
private:
    int count;
    fast_array<T, N> data_;
public:
    inline xvector() : count(0) {
        if(use_xmem) {
            xmem::init(this, sizeof(*this));
        }
    }
    inline void set(int i, const T & o) {
        assert(0<=i && i<count);
        if(use_xmem) {
            xmem::modify(&data_[i], sizeof(data_[i]));
        }
        data_[i] = o;
    }
    inline const T & get(int i) const {
        assert(0<=i && i<count);
        return data_[i];
    }
    inline void clear() {
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        count = 0;
    }
    inline bool empty() const {
        return count==0;
    }
    inline bool full() const {
        return count==N;
    }
    inline int full_size() const {
        return N;
    }
    template<class TyIt> inline void assign(TyIt st, TyIt et) {
        int cnt = 0;
        if(use_xmem && xmem::enabled()) {
            TyIt it = st;
            while(it!=et) {
                assert(cnt<N);
                ++it;
                ++cnt;
            }
            xmem::modify(&count, sizeof(count));
            xmem::modify(data_.data(), sizeof(T)*cnt);
            cnt = 0;
        }
        while(st!=et) {
            assert(cnt<N);
            data_[cnt] = *st;
            ++st;
            ++cnt;
        }
        count = cnt;
    }
    inline void push_back(const T & value) {
        assert(0<=count && count<N);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
            xmem::modify(&data_[count], sizeof(T));
        }
        data_[count] = value;
        ++count;
    }
    template<class... TyArgs> inline void emplace_back(TyArgs&&... args) {
        assert(0<=count && count<N);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
            xmem::modify(&data_[count], sizeof(T));
        }
        ::new(&data_[count])T(std::forward<TyArgs>(args)...);
        ++count;
    }
    inline xref<T> let_back() {
        assert(0<=count && count<N);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        return xref<T>(data_.data() + (count++));
    }
    inline void pop_back() {
        assert(0<count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        --count;
    }
    inline const T & pop_back_with_value() {
        assert(0<count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        --count;
        return data_[count];
    }
    inline void pop_swap_back(int i) {
        assert(0<=i && i<count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        --count;
        if likely(i!=count) {
            if(use_xmem) {
                xmem::modify(&data_[i], sizeof(data_[i]));
            }
            data_[i] = data_[count];
        }
    }
    inline T pop_swap_back_with_value(int i) {
        assert(0<=i && i<count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        --count;
        T ret = data_[i];
        if likely(i!=count) {
            if(use_xmem) {
                xmem::modify(&data_[i], sizeof(data_[i]));
            }
            data_[i] = data_[count];
        }
        return ret;
    }
    inline xref<T> front() {
        assert(0<count);
        return xref<T>(data_.data());
    }
    inline const T & front() const {
        assert(0<count);
        return data_[0];
    }
    inline xref<T> back() {
        assert(0<count);
        return xref<T>(data_.data() + (count-1));
    }
    inline const T & back() const {
        assert(0<count);
        return data_[count-1];
    }
    inline int size() const {
        return count;
    }
    inline void resize(int n, const T & value) {
        assert(0<=n && n<=N);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        if(count<n) {
            if(use_xmem) {
                xmem::modify(&data_[count], sizeof(*data_.data())*(n-count));
            }
            for(int i=count; i<n; ++i) {
                data_[i] = value;
            }
        }
        count = n;
    }
    inline void resize(int n) {
        assert(0<=n && n<=N);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        count = n;
    }
    inline const T * data() const {
        return data_.data();
    }
    inline const T * begin() const {
        return data_.data();
    }
    inline const T * end() const {
        return data_.data()+count;
    }
    inline std::reverse_iterator<const T *> rbegin() const {
        return std::reverse_iterator<const T *>(end());
    }
    inline std::reverse_iterator<const T *> rend() const {
        return std::reverse_iterator<const T *>(begin());
    }
    inline void sort() {
        if(use_xmem) {
            assert(!xmem::enabled());
        }
        std::sort(data_.data(), data_.data()+count);
    }
    inline void replace(const T & from, const T & to) {
        if(from==to) {
            return;
        }
        T * p = std::lower_bound(data_.data(), data_.data()+count, from);
        assert(p!=end());
        assert(*p==from);
        if(from<to) {
            T * e = (T*)end();
            if(use_xmem) {
                T * ep = p+1;
                while(ep<e && *ep<to) {
                    ++ep;
                }
                --ep;
                xmem::modify(p, sizeof(*p)*(ep-p+1));
                while(p<ep) {
                    *p = p[1];
                    ++p;
                }
            }
            else {
                ++p;
                while(p<e && *p<to) {
                    p[-1] = *p;
                    ++p;
                }
                --p;
            }
        }
        else {
            T * b = (T*)begin();
            if(use_xmem) {
                T * ep = p-1;
                while(b<=ep && to<*ep) {
                    --ep;
                }
                ++ep;
                xmem::modify(ep, sizeof(*ep)*(p-ep+1));
                while(ep<p) {
                    *p = p[-1];
                    --p;
                }
            }
            else {
                --p;
                while(b<=p && to<*p) {
                    p[1] = *p;
                    --p;
                }
                ++p;
            }
        }
        *p = to;
    }
    inline xref<T> operator[] (int i) {
        assert(0<=i && i<count);
        return xref<T>(data_.data() + i);
    }
    inline const T & operator[] (int i) const {
        assert(0<=i && i<count);
        return data_[i];
    }
    inline xvector<T, N, use_xmem> & operator = (std::vector<T> & o) {
        assign(o.begin(), o.end());
        return *this;
    }
    bool in_array_slow(const T & value) const {
        for(int i=0; i<count; ++i) {
            if unlikely(data_[i]==value) {
                return true;
            }
        }
        return false;
    }
};
