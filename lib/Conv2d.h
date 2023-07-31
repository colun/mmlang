struct Conv2d {
    std::vector<float> bias;
    std::vector<std::vector<std::vector<std::vector<float> > > > weight;
    int out_channels;
    int in_channels;
    int kernel_h;
    int kernel_w;
    int padding_h;
    int padding_w;
    int stride_y;
    int stride_x;
    void init(const char * bias_s, const char * weight_s) {
        decord_ndarray(bias_s, bias);
        decord_ndarray(weight_s, weight);
        assert(bias.size()==weight.size());
        out_channels = weight.size();
        in_channels = weight[0].size();
        kernel_h = weight[0][0].size();
        kernel_w = weight[0][0][0].size();
        padding_h = (kernel_h-1) / 2;
        padding_w = (kernel_w-1) / 2;
        stride_y = (kernel_h & 1) + 1;
        stride_x = (kernel_w & 1) + 1;
    }
    Conv2d() {
    }
    Conv2d(const char * bias_s, const char * weight_s) {
        init(bias_s, weight_s);
    }
    template<class TI, class TO> void operator() (std::vector<std::vector<std::vector<TI> > > & in_vec, std::vector<std::vector<std::vector<TO> > > & out_vec) {
        int out_h = out_vec.size();
        int out_w = out_vec[0].size();
        int out_c = out_channels;
        int in_h = in_vec.size();
        int in_w = in_vec[0].size();
        int in_c = in_channels;
        int kh = kernel_h;
        int kw = kernel_w;
        int ph = padding_h;
        int pw = padding_w;
        for(int y=0, y2=0; y<out_h; ++y, y2+=stride_y) {
            for(int x=0, x2=0; x<out_w; ++x, x2+=stride_x) {
                std::vector<TO> & out_vec2 = out_vec[y][x];
                for(int c=0; c<out_c; ++c) {
                    TO f = bias[c];
                    std::vector<std::vector<std::vector<float> > > weight2 = weight[c];
                    for(int dy=0; dy<kh; ++dy) {
                        int y3 = y2 + dy - ph;
                        if(y3<0 || in_h<=y3) {
                            continue;
                        }
                        for(int dx=0; dx<kw; ++dx) {
                            int x3 = x2 + dx - pw;
                            if(x3<0 || in_w<=x3) {
                                continue;
                            }
                            std::vector<TI> & in_vec2 = in_vec[y3][x3];
                            for(int c2=0; c2<in_c; ++c2) {
                                f += in_vec2[c2] * weight2[c2][dy][dx];
                            }
                        }
                    }
                    out_vec2[c] = f;
                }
            }
        }
    }
};
