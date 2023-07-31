unsigned long long mrand49$state = 0x8a5cd789635d2dffULL;
#ifdef MRAND49$RESERVE
int mrand49$cursor = -1;
std::vector<int> mrand49$reserved;
inline int mrand49() {
    if(mrand49$cursor!=-1) {
        if(mrand49$cursor<mrand49$reserved.size()) {
            return mrand49$reserved[mrand49$cursor++];
        }
        mrand49$state *= 6364136223846793005ULL;
        mrand49$state += 1442695040888963407ULL;
        unsigned int ret = ((mrand49$state>>18)^mrand49$state) >> 27;
        unsigned int rot = (mrand49$state>>59);
        int ret2 = (ret>>rot) | (ret<<-rot);
        mrand49$reserved.emplace_back(ret2);
        ++mrand49$cursor;
        return ret2;
    }
#else
inline int mrand49() {
#endif
    mrand49$state *= 6364136223846793005ULL;
    mrand49$state += 1442695040888963407ULL;
    unsigned int ret = ((mrand49$state>>18)^mrand49$state) >> 27;
    unsigned int rot = (mrand49$state>>59);
    return (ret>>rot) | (ret<<-rot);
}
inline int mrand49(int r) {
    assert(r);
    return asm_mul_hi(mrand49(), r);
}
