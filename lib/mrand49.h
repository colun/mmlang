unsigned long long mrand49$state = 0x8a5cd789635d2dffULL;
inline int mrand49() {
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
