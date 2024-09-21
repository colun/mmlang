char format$buf[65536];
string format(const char * format, ...) {
    va_list arg;
    va_start(arg, format);
    int len = vsprintf(format$buf, format, arg);
    va_end(arg);
    return format$buf;
}
