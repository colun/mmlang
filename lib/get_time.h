unsigned long long getTime$initCpuClock;
unsigned long long getTime$reserveUpdateCpuClock;
double getTime$initNativeTime;
double getTime$secPerClock;
double getTime$doneTime;
inline void initTime() {
    getTime$initNativeTime = getNativeTime();
    getCpuClock(getTime$initCpuClock);
    getTime$secPerClock = 0.00000000025;
    getTime$reserveUpdateCpuClock = 10000000;
    getTime$doneTime = 0;
}
struct getTime$init_class {
    inline getTime$init_class() {
        initTime();
    }
};
getTime$init_class getTime$init_obj;

#if __local__
double getTime$revRate = 1.0;
double getTime$rate = 1.0;
bool getTime$stopFlag = false;
double getTime$correct = 0.0;
#endif
inline void setLocalTimeRate(double rate) {
#if __local__
    getTime$rate = rate;
    getTime$revRate = 1.0 / rate;
#endif
}

inline double getTime() {
#if __local__
    if(getTime$stopFlag) {
        return getTime$doneTime;
    }
#endif
    unsigned long long now;
    getCpuClock(now);
    now -= getTime$initCpuClock;
    if(getTime$reserveUpdateCpuClock < now) {
        double nowTime = getNativeTime() - getTime$initNativeTime;
        getTime$secPerClock = nowTime / now;
        getTime$reserveUpdateCpuClock = now + (unsigned long long)(0.05 * now / nowTime);
    }
#if __local__
    return (getTime$doneTime = std::fmax(getTime$doneTime, getTime$secPerClock * now * getTime$revRate + getTime$correct));
#else
    return getTime$doneTime = std::fmax(getTime$doneTime, getTime$secPerClock * now);
#endif
}
inline void getTimeStop() {
#if __local__
    if(!getTime$stopFlag) {
        getTime();
        getTime$stopFlag = true;
    }
#endif
}
inline void getTimeStart() {
#if __local__
    if(getTime$stopFlag) {
        double before = getTime();
        getTime$stopFlag = false;
        double after = getTime();
        getTime$correct += before - after;
        getTime$doneTime = before;
    }
#endif
}
inline void setTime(double t) {
#if __local__
    getTime$correct += t - getTime();
    getTime$doneTime = t;
#endif
}
