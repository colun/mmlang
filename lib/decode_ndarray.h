void decord_ndarray_head(const char * & p, std::vector<int> & shape, int & float_chars) {
    shape.clear();
    int s = 0;
    while(true) {
        char c = *p;
        ++p;
        if('0'<=c && c<='9') {
            s = s * 10 + (c - '0');
        }
        else if(c==',') {
            shape.push_back(s);
            s = 0;
        }
        else if(c==':') {
            shape.push_back(s);
            break;
        }
        else {
            assert(false);
        }
    }
    float_chars = 0;
    while(true) {
        char c = *p;
        ++p;
        if('0'<=c && c<='9') {
            float_chars = float_chars * 10 + (c - '0');
        }
        else if(c==':') {
            break;
        }
    }
}
void decord_ndarray(const char * & p, std::vector<float> & vec) {
    std::vector<int> shape;
    int float_chars;
    decord_ndarray_head(p, shape, float_chars);
    assert(2<=float_chars && float_chars<=5);
    assert(shape.size()==1);
    vec.resize(shape[0]);
    for(int i=0, e=shape[0]; i<e; ++i) {
        vec[i] = decode_float(p, float_chars);
    }
    assert(*p=='\0');
}
void decord_ndarray(const char * & p, std::vector<std::vector<std::vector<std::vector<float> > > > & vec) {
    std::vector<int> shape;
    int float_chars;
    decord_ndarray_head(p, shape, float_chars);
    assert(2<=float_chars && float_chars<=5);
    assert(shape.size()==4);
    int ei = shape[0];
    int ej = shape[1];
    int ek = shape[2];
    int el = shape[3];
    vec.resize(ei);
    for(int i=0; i<ei; ++i) {
        vec[i].resize(ej);
        for(int j=0; j<ej; ++j) {
            vec[i][j].resize(ek);
            for(int k=0; k<ek; ++k) {
                std::vector<float> & vec2 = vec[i][j][k];
                vec2.resize(el);
                for(int l=0; l<el; ++l) {
                    vec2[l] = decode_float(p, float_chars);
                }
            }
        }
    }
    assert(*p=='\0');
}
