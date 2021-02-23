unsigned long long getTime$initCpuClock;
unsigned long long getTime$reserveUpdateCpuClock;
double getTime$initNativeTime;
double getTime$secPerClock;
double getTime$doneTime;
struct getTime$init_class {
    inline getTime$init_class() {
        getTime$initNativeTime = getNativeTime();
        getCpuClock(getTime$initCpuClock);
        getTime$secPerClock = 0.00000000025;
        getTime$reserveUpdateCpuClock = 10000000;
        getTime$doneTime = 0;
    }
};
getTime$init_class getTime$init_obj;
inline double getTime() {
    unsigned long long now;
    getCpuClock(now);
    now -= getTime$initCpuClock;
    if(getTime$reserveUpdateCpuClock < now) {
        double nowTime = getNativeTime() - getTime$initNativeTime;
        getTime$secPerClock = nowTime / now;
        getTime$reserveUpdateCpuClock = now + (unsigned long long)(0.05 * now / nowTime);
    }
    return getTime$doneTime = std::fmax(getTime$doneTime, getTime$secPerClock * now);
}
