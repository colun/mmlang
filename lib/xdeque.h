template<typename T, int N, bool use_xmem=true> class xdeque {
private:
    int start;
    int count;
    fast_array<T, N> data_;
    inline int calcPos(int i) {
        assert(0<=start && start<N);
        int pos = start + i;
        return pos<N ? pos : pos-N;
    }
public:
    inline xdeque() : start(0), count(0) {
        if(use_xmem) {
            xmem::init(this);
        }
    }
    inline void set(int i, const T & o) {
        assert(0<=i && i<count);
        int pos = calcPos(i);
        if(use_xmem) {
            xmem::modify(&data_[pos], sizeof(data_[pos]));
        }
        data_[pos] = o;
    }
    inline const T & get(int i) const {
        assert(0<=i && i<count);
        int pos = calcPos(i);
        return data_[pos];
    }
    inline void clear() {
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        start = 0;
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
            xmem::modify(&start, sizeof(start));
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
        start = 0;
        count = cnt;
    }
    inline void push_back(const T & value) {
        assert(0<=count && count<N);
        int pos = calcPos(count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
            xmem::modify(&data_[pos], sizeof(T));
        }
        data_[pos] = value;
        ++count;
    }
    inline void push_front(const T & value) {
        assert(0<=count && count<N);
        assert(0<=start && start<N);
        int pos = start ? start-1 : N-1;
        if(use_xmem) {
            xmem::modify(&start, sizeof(start));
            xmem::modify(&count, sizeof(count));
            xmem::modify(&data_[pos], sizeof(T));
        }
        data_[pos] = value;
        start = pos;
        ++count;
    }
    template<class... TyArgs> inline void emplace_back(TyArgs&&... args) {
        assert(0<=count && count<N);
        int pos = calcPos(count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
            xmem::modify(&data_[pos], sizeof(T));
        }
        ::new(&data_[pos])T(std::forward<TyArgs>(args)...);
        ++count;
    }
    template<class... TyArgs> inline void emplace_front(TyArgs&&... args) {
        assert(0<=count && count<N);
        assert(0<=start && start<N);
        int pos = start ? start-1 : N-1;
        if(use_xmem) {
            xmem::modify(&start, sizeof(start));
            xmem::modify(&count, sizeof(count));
            xmem::modify(&data_[pos], sizeof(T));
        }
        ::new(&data_[pos])T(std::forward<TyArgs>(args)...);
        start = pos;
        ++count;
    }
    inline xref<T> let_back() {
        assert(0<=count && count<N);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        return xref<T>(data_.data() + calcPos(count++));
    }
    inline xref<T> let_front() {
        assert(0<=count && count<N);
        assert(0<=start && start<N);
        if(use_xmem) {
            xmem::modify(&start, sizeof(start));
            xmem::modify(&count, sizeof(count));
        }
        start = start ? start-1 : N-1;
        ++count;
        return xref<T>(data_.data() + start);
    }
    inline void pop_back() {
        assert(0<count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        --count;
    }
    inline void pop_front() {
        assert(0<count);
        assert(0<=start && start<N);
        if(use_xmem) {
            xmem::modify(&start, sizeof(start));
            xmem::modify(&count, sizeof(count));
        }
        start = start==N-1 ? 0 : start+1;
        --count;
    }
    inline const T & pop_back_with_value() {
        assert(0<count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        --count;
        return data_[calcPos(count)];
    }
    inline const T & pop_front_with_value() {
        assert(0<count);
        assert(0<=start && start<N);
        if(use_xmem) {
            xmem::modify(&start, sizeof(start));
            xmem::modify(&count, sizeof(count));
        }
        int pos = start;
        start = start==N-1 ? 0 : start+1;
        --count;
        return data_[pos];
    }
    inline void pop_swap_back(int i) {
        assert(0<=i && i<count);
        if(use_xmem) {
            xmem::modify(&count, sizeof(count));
        }
        --count;
        if likely(i!=count) {
            int pos = calcPos(i);
            if(use_xmem) {
                xmem::modify(&data_[pos], sizeof(data_[pos]));
            }
            data_[pos] = data_[calcPos(count)];
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
            int pos = calcPos(i);
            if(use_xmem) {
                xmem::modify(&data_[pos], sizeof(data_[pos]));
            }
            data_[pos] = data_[calcPos(count)];
        }
        return ret;
    }
    inline xref<T> front() {
        assert(0<count);
        return xref<T>(data_.data() + start);
    }
    inline const T & front() const {
        assert(0<count);
        return data_[start];
    }
    inline xref<T> back() {
        assert(0<count);
        return xref<T>(data_.data() + calcPos(count-1));
    }
    inline const T & back() const {
        assert(0<count);
        return data_[calcPos(count-1)];
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
            int boundary = N - start;
            if(n<=boundary) {
                if(use_xmem) {
                    xmem::modify(&data_[count+start], sizeof(*data_.data())*(n-count));
                }
                for(int i=count; i<n; ++i) {
                    data_[i+start] = value;
                }
            }
            else if(boundary <= count) {
                if(use_xmem) {
                    xmem::modify(&data_[count+start-N], sizeof(*data_.data())*(n-count));
                }
                for(int i=count; i<n; ++i) {
                    data_[i+start-N] = value;
                }
            }
            else {
                if(use_xmem) {
                    xmem::modify(&data_[count+start], sizeof(*data_.data())*(boundary-count));
                    xmem::modify(&data_[count+start-N], sizeof(*data_.data())*(n-boundary));
                }
                for(int i=count; i<boundary; ++i) {
                    data_[i+start] = value;
                }
                for(int i=boundary; i<n; ++i) {
                    data_[i+start-N] = value;
                }
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
    class const_iterator {
        int index;
        const xdeque * target;
    public:
        inline const_iterator(xdeque * target, int i) : target(target), index(i) {}
        inline const T & operator * () const {
            return target->data_[target->calcPos(index)];
        }
        inline const_iterator & operator++() {
            ++index;
            return *this;
        }
        inline const_iterator operator++(int) {
            ++index;
            return const_iterator(target, index-1);
        }
        inline const_iterator & operator--() {
            --index;
            return *this;
        }
        inline const_iterator operator--(int) {
            --index;
            return const_iterator(target, index+1);
        }
        inline bool operator==(const const_iterator & o) const {
            return index==o.index && target==o.target;
        }
        inline bool operator!=(const const_iterator & o) const {
            return index!=o.index || target!=o.target;
        }
    };
    inline const const_iterator begin() const {
        return const_iterator(this, 0);
    }
    inline const const_iterator end() const {
        return const_iterator(this, count);
    }
    inline std::reverse_iterator<const_iterator> rbegin() const {
        return std::reverse_iterator<const_iterator>(end());
    }
    inline std::reverse_iterator<const_iterator> rend() const {
        return std::reverse_iterator<const_iterator>(begin());
    }
    inline xref<T> operator[] (int i) {
        assert(0<=i && i<count);
        return xref<T>(data_.data() + calcPos(i));
    }
    inline const T & operator[] (int i) const {
        assert(0<=i && i<count);
        return data_[calcPos(i)];
    }
};
