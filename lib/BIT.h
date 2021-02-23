template<class T> class BIT {
private:
    std::vector<T> data;
public:
    inline BIT(int n=0) : data(n) { assert(0<=n); }
    inline int size() {
        return (int)data.size();
    }
    inline void resize(int n) {
        data.resize(n);
    }
    inline void clear() {
        data.clear();
    }
    inline void add(int i, T o=1) {
        assert(0<=i);
        assert(i<(int)data.size());
        if(o==0) {
            return;
        }
        data[i] += o;
        while(i) {
            i &= i-1;
            data[i] += o;
        }
    }
    inline T sum_back(int i) const {
        if(i) {
            T ret = 0;
            while(i<(int)data.size()) {
                ret += data[i];
                i += i&-i;
            }
            return ret;
        }
        else {
            return data[0];
        }
    }
    inline T sum(int i) const {
        assert(0<=i);
        assert(i<=(int)data.size());
        return data[0] - sum_back(i);
    }
    inline T sum(int l, int r) const {
        assert(0<=l);
        assert(l<=(int)data.size());
        assert(0<=r);
        assert(r<=(int)data.size());
        return sum_back(l) - sum_back(r);
    }
    inline T get(int i) const {
        assert(0<=i);
        assert(i<(int)data.size());
        T ret = data[i];
        int j = 1;
        int i2 = ~i;
        while(i2&j) {
            int ij = i+j;
            if((int)data.size()<=ij) {
                break;
            }
            ret -= data[ij];
            j += j;
        }
        return ret;
    }
    class ref {
    private:
        BIT * bit;
        int i;
    public:
        inline ref(BIT * bit, int i) : bit(bit), i(i) {}
        inline void operator = (const T & o) {
            bit->add(i, o - bit->get(i));
        }
        inline void operator += (const T & o) {
            bit->add(i, o);
        }
        inline void operator -= (const T & o) {
            bit->add(i, -o);
        }
        inline void operator *= (const T & o) {
            bit->add(i, bit->get(i) * (o-1));
        }
        inline void operator /= (const T & o) {
            T v = bit->get(i);
            bit->add(i, v / o - v);
        }
        inline void operator ++ () {
            bit->add(i, 1);
        }
        inline void operator ++ (int) {
            bit->add(i, 1);
        }
        inline void operator -- () {
            bit->add(i, -1);
        }
        inline void operator -- (int) {
            bit->add(i, -1);
        }
        inline operator T() const {
            return bit->get(i);
        }
    };
    inline ref operator[] (int i) {
        assert(0<=i);
        assert(i<(int)data.size());
        return ref(this, i);
    }
    inline T operator[] (int i) const {
        assert(0<=i);
        assert(i<(int)data.size());
        return get(i);
    }
};
