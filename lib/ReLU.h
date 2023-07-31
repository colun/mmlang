template<class T> void ReLU(std::vector<std::vector<std::vector<T> > > & in_vec) {
    int ey = in_vec.size();
    int ex = in_vec[0].size();
    int ec = in_vec[0][0].size();
    for(int y=0; y<ey; ++y) {
        std::vector<std::vector<T> > & vec2 = in_vec[y];
        for(int x=0; x<ex; ++x) {
            std::vector<T> & vec3 = vec2[x];
            for(int c=0; c<ec; ++c) {
                if(vec3[c]<0.0f) {
                    vec3[c] = 0.0f;
                }
            }
        }
    }
}
