struct BatchNorm2d {
    std::vector<float> bias;
    std::vector<float> weight;
    void init(const char * bias_s, const char * weight_s) {
        decord_ndarray(bias_s, bias);
        decord_ndarray(weight_s, weight);
        assert(bias.size()==weight.size());
    }
    BatchNorm2d() {
    }
    BatchNorm2d(const char * bias_s, const char * weight_s) {
        init(bias_s, weight_s);
    }
    template<class T> void operator() (std::vector<std::vector<std::vector<T> > > & in_vec) {
        int ey = in_vec.size();
        int ex = in_vec[0].size();
        int ec = bias.size();
        for(int y=0; y<ey; ++y) {
            std::vector<std::vector<T> > & vec2 = in_vec[y];
            for(int x=0; x<ex; ++x) {
                std::vector<T> & vec3 = vec2[x];
                assert(vec3.size()==ec);
                for(int c=0; c<ec; ++c) {
                    vec3[c] = vec3[c] * weight[c] + bias[c];
                }
            }
        }
    }
};
