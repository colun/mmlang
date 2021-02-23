template<typename T, int N> class fast_array {
private:
    char data_[sizeof(T)*N];
public:
    inline T* data() {
        return (T*)data_;
    }
    inline const T* data() const {
        return (const T*)data_;
    }
    inline std::size_t sizeofData() const {
        return sizeof(data_);
    }
    inline int size() const {
        return N;
    }
    inline T* begin() {
        return (T*)data_;
    }
    inline const T* begin() const {
        return (const T*)data_;
    }
    inline T* end() {
        return ((T*)data_) + N;
    }
    inline const T* end() const {
        return ((const T*)data_) + N;
    }
    inline T & operator[] (int i) {
        assert(0<=i && i<N);
        return ((T*)data_)[i];
    }
    inline const T & operator[] (int i) const {
        assert(0<=i && i<N);
        return ((const T*)data_)[i];
    }
};
