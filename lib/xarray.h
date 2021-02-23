template<class T, int N> class xarray {
private:
    fast_array<T, N> data_;
public:
    inline xarray() {
        xmem::init(this);
    }
    inline void set(int i, const T & o) {
        assert(0<=i && i<N);
        xmem::modify(&data_[i], sizeof(data_[i]));
        data_[i] = o;
    }
    inline const T & get(int i) const {
        assert(0<=i && i<N);
        return data_[i];
    }
    inline const T* data() const {
        return data_.data();
    }
    inline int size() const {
        return N;
    }
    inline const T* begin() const {
        return data_.begin();
    }
    inline const T* end() const {
        return data_.end();
    }
    inline xref<T> operator[] (int i) {
        assert(0<=i && i<N);
        return xref<T>(&data_[i]);
    }
    inline const T & operator[] (int i) const {
        assert(0<=i && i<N);
        return data_[i];
    }
};
