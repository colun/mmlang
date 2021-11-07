static inline int countBit32(unsigned int n) {
	n = (n&0x55555555) + ((n>>1)&0x55555555);
	n = (n&0x33333333) + ((n>>2)&0x33333333);
	n = (n + (n>>4)) & 0x0F0F0F0F;
	n += n>>8;
	return (n + (n>>16)) & 0xFF;
}
