template<typename T, int N> class zobrist {
private:
    T hash[N];
public:
    zobrist() {
        if(8<=sizeof(T)) {
            for(int i=0; i<N; ++i) {
                hash[i] = (T)mmrand49();
            }
        }
        else {
            for(int i=0; i<N; ++i) {
                hash[i] = (T)mrand49();
            }
        }
    }
    inline const T & operator[] (int i) const {
        assert(0<=i && i<N);
        return hash[i];
    }
};
