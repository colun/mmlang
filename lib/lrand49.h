inline int lrand49() {
    return mrand49() & 0x7FFFFFFF;
}
inline int lrand49(int r) {
    assert(1<=r);
    return asm_mul_hi(mrand49(), r);
}
