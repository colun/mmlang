class axis2d {
    int width_;
    int height_;
    bool tilt_;
    int pad_;
    int begin_;
    int end_;
    int size_;
    std::vector<int> y_;
    std::vector<int> x_;
    std::vector<bool> in_;
    std::vector<int> ulrd_;
public:
    static constexpr int dx = 1;
    int dy;
    inline axis2d() {}
    inline axis2d(int width, int height=0, bool tilt=false, int pad=1) : width_(width), height_(height), tilt_(tilt), pad_(pad) {
        if(height==0) {
            height_ = height = width;
        }
        dy = width + pad;
        if(tilt) {
            begin_ = (dy+1) * pad;
        }
        else {
            begin_ = dy * pad;
        }
        end_ = begin_ + dy * (height-1) + width;
        size_ = end_ + begin_;
        y_.resize(size_);
        x_.resize(size_);
        in_.resize(size_);
        for(int i=0; i<size_; ++i) {
            x_[i] = -1;
            y_[i] = -1;
            in_[i] = false;
        }
        for(int y=0, pos1=begin_; y<height; ++y, pos1+=dy) {
            for(int x=0, pos=pos1; x<width; ++x, ++pos) {
                x_[pos] = x;
                y_[pos] = y;
                in_[pos] = true;
            }
        }
        ulrd_.resize(4);
        ulrd_[0] = -dy;
        ulrd_[1] = -1;
        ulrd_[2] = 1;
        ulrd_[3] = dy;
    }
    inline int width() const {
        return width_;
    }
    inline int height() const {
        return height_;
    }
    inline int begin() const {
        return begin_;
    }
    inline int end() const {
        return end_;
    }
    inline int size() const {
        return size_;
    }
    inline int y(int pos) const {
        assert(0<=pos<size_);
        return y_[pos];
    }
    inline int x(int pos) const {
        assert(0<=pos<size_);
        return x_[pos];
    }
    inline bool in(int pos) const {
        assert(0<=pos<size_);
        return in_[pos];
    }
    inline const vector<int> & ulrd() const {
        return ulrd_;
    }
    inline int move(int pos, int dir) const {
        return pos + dir;
    }
    inline bool able(int pos, int dir) const {
        assert(in(pos));
        assert(1<=pad_);
        return in(move(pos, dir));
    }
    inline int operator()(int x, int y) const {
        return begin_ + dy * y + x;
    }
};
