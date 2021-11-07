static inline int countBit64(unsigned long long n) {
	n = (n&0x5555555555555555ll) + ((n>>1)&0x5555555555555555ll);
	n = (n&0x3333333333333333ll) + ((n>>2)&0x3333333333333333ll);
	n = (n + (n>>4)) & 0x0F0F0F0F0F0F0F0Fll;
	n += n>>8;
    n += n>>16;
	return (int)((n + (n>>32)) & 0xFF);
}
