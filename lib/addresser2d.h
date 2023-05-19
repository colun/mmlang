class addresser2d {
    int width_;
    int height_;
    int y_stride;
    int begin_pos;
    int end_pos;
    int fin_pos;
public:
    inline addresser2d() {}
    inline addresser2d(int width, int height=0, bool diagonal=false, int pad=1) : width_(width), height_(height) {
        if(height==0) {
            height = width;
        }
        y_stride = width + pad;
        if(diagonal) {
            begin_pos = (y_stride+1) * pad;
        }
        else {
            begin_pos = y_stride * pad;
        }
        end_pos = begin_pos + y_stride * (height-1) + width;
        fin_pos = end_pos + begin_pos;
    }
    inline int width() const {
        return width_;
    }
    inline int height() const {
        return height_;
    }
    inline int stride() const {
        return y_stride;
    }
    inline int begin() const {
        return begin_pos;
    }
    inline int end() const {
        return end_pos;
    }
    inline int fin() const {
        return fin_pos;
    }
    inline int address(int x, int y) const {
        return begin_pos + y_stride * y + x;
    }
};
