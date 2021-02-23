//#define NDEBUG
#ifdef mmlang_source_83_lines

W = 30
WW = W * W
 
A: vector!(char)
A[:WW] = inputInt()
P: vector!(short)
C[:WW][:5][:0] = char(0)
R[:WW] = char(0)
S: vector!(char)
S[:WW] = 0
D = [0, -1, +1, -W, +W]
for i in range(WW):
    x, y = i%W, i/W
    c = 0
    cnt = 0
    if 0<x and A[i-1] < A[i]:
        C[i][:5].push_back(1)
        C[i][1].pop_back()
        S[i] = 1
        ++cnt
    if x<W-1 and A[i+1] < A[i]:
        C[i][:5].push_back(2)
        C[i][2].pop_back()
        S[i] = 2
        ++cnt
    if 0<y and A[i-W] < A[i]:
        C[i][:5].push_back(3)
        C[i][3].pop_back()
        S[i] = 3
        ++cnt
    if y<W-1 and A[i+W] < A[i]:
        C[i][:5].push_back(4)
        C[i][4].pop_back()
        S[i] = 4
        ++cnt
    if S[i]:
        ++R[i+D[S[i]]]
    if 2<=cnt:
        P.push_back(i)
limit = 9.8
T0 = 10
while True:
    t = getTime()
    if limit<=t:
        break
    progress = t / limit
    remain = 1 - progress
    T = T0 * remain
    if gvFps(t):
        gvArrow($1%W, $1/W, (n@$1+D[S[:WW]])%W, n/W, gvRGB(128, 128, 128))
        gvText($1%W, $1/W, 0.3, "%d", A[:WW])
        gvTextLeft(W, 0, "t: %f", t)
        gvTextLeft(W, 1, "T: %f", T)
    i = P[lrand49(P.size())]
    oldS = S[i]
    newS = C[i][oldS][lrand49(C[i][oldS].size())]
    oldJ = i+D[oldS]
    newJ = i+D[newS]
    dSc = 0
    if 1==R[oldJ]:
        dSc -= A[oldJ]
    if 0==R[newJ]:
        dSc += A[newJ]
    if drand49()<exp(dSc/T):
        S[i] = newS
        --R[oldJ]
        ++R[newJ]
for i0 in range(WW):
    if R[i0]!=0:
        continue
    while A[i0]:
        i = i0
        while True:
            if not A[i]:
                break
            --A[i]
            print(i/W+1, i%W+1)
            if not S[i]:
                break
            j = i + D[S[i]]
            if A[i]!=A[j]:
                break
            i = j

#endif

#include <cassert>
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/time.h>
#include <cmath>
#include <cstdarg>

using namespace std;

// library code:

typedef long long int64;typedef __int128 int128;const char*inputCLineOrWord(int mode){static char buf[2097152];static int bufLen=sizeof(buf);static int bufPos=sizeof(buf);static bool canReadFlag=true;static bool crFlag=false;static bool enterFlag=false;
if(canReadFlag&&(enterFlag?bufLen<=bufPos:(int)sizeof(buf)<=bufPos+bufPos)){if(0<bufLen-bufPos){memmove(buf,buf+bufPos,bufLen-bufPos);bufLen-=bufPos;}else{bufLen=0;}char*result=fgets(buf+bufLen,sizeof(buf)-bufLen,stdin);canReadFlag=(result!=NULL);if(result!=NULL)
{int n=strlen(result);enterFlag=(n!=(int)sizeof(buf)-1-bufLen||(1<=bufLen+n&&buf[bufLen+n-1]=='\n'));bufLen+=n;}bufPos=0;}if(mode==0){int pos=bufPos;while(true){char c=buf[pos];if(c==32){buf[pos++]='\0';break;}else if(c==10){if(crFlag){crFlag=false;if(bufPos==pos)
{pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret=buf+bufPos;bufPos=pos;while(true){char c=buf[bufPos];if(c==32||c==10||c==13){++bufPos;}else{break;}}return ret;
}else if(mode==1){int pos=bufPos;while(true){char c=buf[pos];if(c==10){if(crFlag){crFlag=false;if(bufPos==pos){pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret
=buf+bufPos;bufPos=pos;if(crFlag){while(true){char c=buf[bufPos];if(c==13){++bufPos;crFlag=false;break;}else{break;}}}return ret;}else if(mode==2){return bufLen<=bufPos?NULL:buf+bufPos;}assert(false);return NULL;}const char*inputCWord(){return inputCLineOrWord(0);
}int inputInt(){return atoi(inputCWord());}struct _range_ite{int i;int s;_range_ite(int i,int s):i(i),s(s){}bool operator !=(const _range_ite&o)const{return i!=o.i;}void operator++(){i+=s;}const int&operator*()const{return i;}};struct _range_t{int b;
int e;int s;_range_t(int b,int e,int s):b(b),e(e),s(s){}_range_ite begin(){return _range_ite(b,s);}_range_ite end(){return _range_ite(e,s);}};_range_t range(int i){return _range_t(0,std::max(0,i),1);}_range_t range(int b,int e){return _range_t(b,std::max(b,
e),1);}_range_t range(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(b,e,s);}_range_t rrange(int i){return _range_t(std::max(0,i)-1,-1,-1);}_range_t rrange(int b,int e){return
_range_t(std::max(b,e)-1,b-1,-1);}_range_t rrange(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(e-s,b-s,-s);}inline void getCpuClock(unsigned long long&t){__asm__ __volatile__
("rdtsc":"=a"(*(unsigned int*)&t),"=d"(((unsigned int*)&t)[1]));}inline double getNativeTime(){timeval tv;gettimeofday(&tv,0);return tv.tv_sec+tv.tv_usec*1e-6;}unsigned long long getTime$initCpuClock;unsigned long long getTime$reserveUpdateCpuClock;double
getTime$initNativeTime;double getTime$secPerClock;double getTime$doneTime;struct getTime$init_class{inline getTime$init_class(){getTime$initNativeTime=getNativeTime();getCpuClock(getTime$initCpuClock);getTime$secPerClock=0.00000000025;getTime$reserveUpdateCpuClock
=10000000;getTime$doneTime=0;}};getTime$init_class getTime$init_obj;inline double getTime(){unsigned long long now;getCpuClock(now);now-=getTime$initCpuClock;if(getTime$reserveUpdateCpuClock<now){double nowTime=getNativeTime()-getTime$initNativeTime;
getTime$secPerClock=nowTime / now;getTime$reserveUpdateCpuClock=now+(unsigned long long)(0.05*now / nowTime);}return getTime$doneTime=std::fmax(getTime$doneTime,getTime$secPerClock*now);}struct gvRGB{int r;int g;int b;gvRGB(int r,int g,int b):r(r),g(g),
b(b){}int toInt()const{return((r&255)<<16)|((g&255)<<8)|(b&255);}};
#ifdef GV_ENABLE
FILE*gv$file=NULL;void gvInit(){if(gv$file==NULL){gv$file=fopen("result.gv","w+");}}void gvFlush(){assert(gv$file);fflush(gv$file);}inline void gvTime(){gvInit();fprintf(gv$file,"n\n");gvFlush();}inline void gvTime(double t){gvInit();fprintf(gv$file,"n %g\n"
,t);gvFlush();}double gv$fps=-1000000000.0;inline bool gvFps(double t,double fps=60.0){if(gv$fps<=t){gv$fps=t+1.0 / fps;gvTime(t);return true;}return false;}void gvText(double x,double y,double r,gvRGB rgb,const char*format="?",...){gvInit();fprintf(gv$file,"t %g %g %d %g "
,x,y,rgb.toInt(),r);va_list arg;va_start(arg,format);vfprintf(gv$file,format,arg);va_end(arg);fprintf(gv$file,"\n");gvFlush();}void gvText(double x,double y,double r,const char*format="?",...){gvInit();fprintf(gv$file,"t %g %g 0 %g ",x,y,r);va_list arg;
va_start(arg,format);vfprintf(gv$file,format,arg);va_end(arg);fprintf(gv$file,"\n");gvFlush();}void gvText(double x,double y,gvRGB rgb,const char*format="?",...){gvInit();fprintf(gv$file,"t %g %g %d 0.5 ",x,y,rgb.toInt());va_list arg;va_start(arg,format);
vfprintf(gv$file,format,arg);va_end(arg);fprintf(gv$file,"\n");gvFlush();}void gvText(double x,double y,const char*format="?",...){gvInit();fprintf(gv$file,"t %g %g 0 0.5 ",x,y);va_list arg;va_start(arg,format);vfprintf(gv$file,format,arg);va_end(arg);
fprintf(gv$file,"\n");gvFlush();}void gvTextLeft(double x,double y,double r,gvRGB rgb,const char*format="?",...){gvInit();fprintf(gv$file,"tl %g %g %d %g ",x,y,rgb.toInt(),r);va_list arg;va_start(arg,format);vfprintf(gv$file,format,arg);va_end(arg);fprintf(gv$file,"\n"
);gvFlush();}void gvTextLeft(double x,double y,double r,const char*format="?",...){gvInit();fprintf(gv$file,"tl %g %g 0 %g ",x,y,r);va_list arg;va_start(arg,format);vfprintf(gv$file,format,arg);va_end(arg);fprintf(gv$file,"\n");gvFlush();}void gvTextLeft(double
x,double y,gvRGB rgb,const char*format="?",...){gvInit();fprintf(gv$file,"tl %g %g %d 0.5 ",x,y,rgb.toInt());va_list arg;va_start(arg,format);vfprintf(gv$file,format,arg);va_end(arg);fprintf(gv$file,"\n");gvFlush();}void gvTextLeft(double x,double y,
const char*format="?",...){gvInit();fprintf(gv$file,"tl %g %g 0 0.5 ",x,y);va_list arg;va_start(arg,format);vfprintf(gv$file,format,arg);va_end(arg);fprintf(gv$file,"\n");gvFlush();}void gvArrow(double x1,double y1,double x2,double y2,double r,gvRGB rgb)
{double odx=x2-x1;double ody=y2-y1;if(fabs(odx)<=1e-9&&fabs(ody)<=1e-9){return;}double rate=r / sqrt(odx*odx+ody*ody);double dx=odx*rate;double dy=ody*rate;double x2_base=x2+dx*0.1;double y2_base=y2+dy*0.1;double dx0=dx*0.1*tan(M_PI*15/180);double dy0
=dy*0.1*tan(M_PI*15/180);double x2_3=x2_base-dx*(0.1/sin(M_PI*15/180));double y2_3=y2_base-dy*(0.1/sin(M_PI*15/180));double x2_4=x2_3-dx*(0.05/tan(M_PI*15/180));double y2_4=y2_3-dy*(0.05/tan(M_PI*15/180));double x2_5=x2_base-dx*(1.0*cos(M_PI*15/180));
double y2_5=y2_base-dy*(1.0*cos(M_PI*15/180));double x2_6=x2_5-dx*(0.1*sin(M_PI*15/180));double y2_6=y2_5-dy*(0.1*sin(M_PI*15/180));double dx5=dx*(1.0*sin(M_PI*15/180));double dy5=dy*(1.0*sin(M_PI*15/180));double dx6=dx5-dx*(0.1*cos(M_PI*15/180));double
dy6=dy5-dy*(0.1*cos(M_PI*15/180));gvInit();fprintf(gv$file,"p %d",rgb.toInt());fprintf(gv$file," %g %g",x2-dy0,y2+dx0);fprintf(gv$file," %g %g",x2_5-dy5,y2_5+dx5);fprintf(gv$file," %g %g",x2_6-dy6,y2_6+dx6);fprintf(gv$file," %g %g",x2_4-dy*0.05,y2_4+dx*0.05);
fprintf(gv$file," %g %g",x1+dx*(0.05*sqrt(2)/(1+sqrt(2)))-dy*0.05,y1+dy*(0.05*sqrt(2)/(1+sqrt(2)))+dx*0.05);fprintf(gv$file," %g %g",x1-dy*(0.05/(1+sqrt(2))),y1+dx*(0.05/(1+sqrt(2))));fprintf(gv$file," %g %g",x1+dy*(0.05/(1+sqrt(2))),y1-dx*(0.05/(1+sqrt(2))));
fprintf(gv$file," %g %g",x1+dx*(0.05*sqrt(2)/(1+sqrt(2)))+dy*0.05,y1+dy*(0.05*sqrt(2)/(1+sqrt(2)))-dx*0.05);fprintf(gv$file," %g %g",x2_4+dy*0.05,y2_4-dx*0.05);fprintf(gv$file," %g %g",x2_6+dy6,y2_6-dx6);fprintf(gv$file," %g %g",x2_5+dy5,y2_5-dx5);fprintf(gv$file," %g %g"
,x2+dy0,y2-dx0);fprintf(gv$file,"\n");gvFlush();}void gvArrow(double x1,double y1,double x2,double y2,double r){gvArrow(x1,y1,x2,y2,r,gvRGB(0,0,0));}void gvArrow(double x1,double y1,double x2,double y2,gvRGB rgb){gvArrow(x1,y1,x2,y2,0.5,rgb);}void gvArrow(double
x1,double y1,double x2,double y2){gvArrow(x1,y1,x2,y2,0.5);}
#else
inline bool gvFps(double t,double fps=60.0){return false;}
#define gvTime(...)
#define gvText(...)
#define gvArrow(...)
#endif
unsigned long long mrand49$state=0x8a5cd789635d2dffULL;inline int mrand49(){mrand49$state*=6364136223846793005ULL;mrand49$state+=1442695040888963407ULL;unsigned int ret=((mrand49$state>>18)^mrand49$state)>> 27;unsigned int rot=(mrand49$state>>59);return
(ret>>rot)|(ret<<-rot);}inline unsigned int asm_mul_hi(unsigned int x,unsigned int y){__asm__ __volatile__("mul %%edx":"+a"(x),"+d"(y));return y;}inline unsigned int asm_mul_hi(unsigned int x){unsigned int y;__asm__ __volatile__("mul %%eax":"+a"(x),"=d"
(y));return y;}inline int lrand49(){return mrand49()&0x7FFFFFFF;}inline int lrand49(int r){assert(1<=r);return asm_mul_hi(mrand49(),r);}inline double drand49(){return((unsigned int)mrand49()+0.5)*(1.0/4294967296.0);}void print_unit(bool val){printf("%s"
,val?"true":"false");}void print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val){printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void print_unit(const void*val){printf("%p"
,val);}void print_unit(long long val){printf("%lld",val);}void print_unit(__int128 val){char buf[128];int idx=128;buf[--idx]='\0';bool sign=false;if(val<0){sign=true;val=-val;}if(val==0){buf[--idx]='0';}else while(val){buf[--idx]='0'+(val % 10);val /=
10;}if(sign){buf[--idx]='-';}printf("%s",buf+idx);}void print_unit(double val){printf("%g",val);}void print_unit(const char*val){printf("%s",val);}void print_unit(const std::string&val){printf("%s",val.c_str());}template<class T>void print_unit(const
std::vector<T>&val){printf("vec(%d) {",(int)val.size());for(int i=0;i<(int)val.size();++i){fputc(' ',stdout);print_unit(val[i]);}printf(" }");}
#define _print0()
#define _print1(e)print_unit(e)
#define _print2(e1,e2)_print1(e1),fputc(' ',stdout),_print1(e2)
#define _print3(e1,e2,e3)_print2(e1,e2),fputc(' ',stdout),_print1(e3)
#define _print4(e1,e2,e3,e4)_print2(e1,e2),fputc(' ',stdout),_print2(e3,e4)
#define _print5(e1,e2,e3,e4,e5)_print3(e1,e2,e3),fputc(' ',stdout),_print2(e4,e5)
#define _print6(e1,e2,e3,e4,e5,e6)_print3(e1,e2,e3),fputc(' ',stdout),_print3(e4,e5,e6)
#define _print7(e1,e2,e3,e4,e5,e6,e7)_print4(e1,e2,e3,e4),fputc(' ',stdout),_print3(e5,e6,e7)
#define _print8(e1,e2,e3,e4,e5,e6,e7,e8)_print4(e1,e2,e3,e4),fputc(' ',stdout),_print4(e5,e6,e7,e8)
#define _print9(e1,e2,e3,e4,e5,e6,e7,e8,e9)_print5(e1,e2,e3,e4,e5),fputc(' ',stdout),_print4(e6,e7,e8,e9)
#define _print10(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10)_print5(e1,e2,e3,e4,e5),fputc(' ',stdout),_print5(e6,e7,e8,e9,e10)
#define _GET_PRINT_MACRO_NAME(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...)NAME
#define print(...)_GET_PRINT_MACRO_NAME(__VA_ARGS__,_print10,_print9,_print8,_print7,_print6,_print5,_print4,_print3,_print2,_print1,_print0)(__VA_ARGS__),fputc('\n',stdout)
#define print0(...)_GET_PRINT_MACRO_NAME(__VA_ARGS__,_print10,_print9,_print8,_print7,_print6,_print5,_print4,_print3,_print2,_print1,_print0)(__VA_ARGS__)

// generated code (by mmlang ... https://github.com/colun/mmlang ) :

int W;
int WW;
vector<char> A;
vector<short> P;
vector<vector<vector<char> > > C;
vector<int> R;
vector<char> S;
vector<int> D;
int x;
int y;
int c;
int cnt;
double limit;
int T0;
double t;
double progress;
double remain;
double T;
int i;
char oldS;
char newS;
int oldJ;
int newJ;
int dSc;
int j;

int main() {
    W = 30;
    WW = W * W;
    {
        A.resize(max((int)A.size(), (int)WW));
        for(int $1=0, $e=(0<=WW ? WW : (int)A.size()+WW); $1<$e; ++$1) A[$1] = inputInt();
    }
    {
        C.resize(max((int)C.size(), (int)WW));
        for(int $1=0, $e=(0<=WW ? WW : (int)C.size()+WW); $1<$e; ++$1) {
            C[$1].resize(max((int)C[$1].size(), (int)5));
            for(int $2=0, $e=(0<=5 ? 5 : (int)C[$1].size()+5); $2<$e; ++$2) {
                C[$1][$2].resize(max((int)C[$1][$2].size(), (int)0));
                for(int $3=0, $e=(0<=0 ? 0 : (int)C[$1][$2].size()+0); $3<$e; ++$3) C[$1][$2][$3] = char(0);
            }
        }
    }
    {
        R.resize(max((int)R.size(), (int)WW));
        for(int $1=0, $e=(0<=WW ? WW : (int)R.size()+WW); $1<$e; ++$1) R[$1] = char(0);
    }
    {
        S.resize(max((int)S.size(), (int)WW));
        for(int $1=0, $e=(0<=WW ? WW : (int)S.size()+WW); $1<$e; ++$1) S[$1] = 0;
    }
    {
        const auto & $c1 = -1;
        const auto & $c2 = +1;
        const auto & $c3 = -W;
        const auto & $c4 = +W;
        D = vector<int>({0, $c1, $c2, $c3, $c4});
    }
    for(const int & i : range(WW)) {
        {
            const auto & $c1 = i % W;
            const auto & $c2 = i / W;
            tie(x, y) = tuple<int, int>($c1, $c2);
        }
        c = 0;
        cnt = 0;
        if(0 < x && A[i - 1] < A[i]) {
            for(int $1=0, $e=(0<=5 ? 5 : (int)C[i].size()+5); $1<$e; ++$1) C[i][$1].push_back(1);
            C[i][1].pop_back();
            S[i] = 1;
            ++cnt;
        }
        if(x < W - 1 && A[i + 1] < A[i]) {
            for(int $1=0, $e=(0<=5 ? 5 : (int)C[i].size()+5); $1<$e; ++$1) C[i][$1].push_back(2);
            C[i][2].pop_back();
            S[i] = 2;
            ++cnt;
        }
        if(0 < y && A[i - W] < A[i]) {
            for(int $1=0, $e=(0<=5 ? 5 : (int)C[i].size()+5); $1<$e; ++$1) C[i][$1].push_back(3);
            C[i][3].pop_back();
            S[i] = 3;
            ++cnt;
        }
        if(y < W - 1 && A[i + W] < A[i]) {
            for(int $1=0, $e=(0<=5 ? 5 : (int)C[i].size()+5); $1<$e; ++$1) C[i][$1].push_back(4);
            C[i][4].pop_back();
            S[i] = 4;
            ++cnt;
        }
        if(S[i]) {
            ++R[i + D[S[i]]];
        }
        if(2 <= cnt) {
            P.push_back(i);
        }
    }
    limit = 9.8;
    T0 = 10;
    while(true) {
        t = getTime();
        if(limit <= t) {
            goto $L2;
        }
        progress = t / limit;
        remain = 1 - progress;
        T = T0 * remain;
        if(gvFps(t)) {
            for(int $1=0, $e=(0<=WW ? WW : (int)S.size()+WW); $1<$e; ++$1) {
                const auto & n = $1 + D[S[$1]];
                gvArrow($1 % W, $1 / W, (n) % W, n / W, gvRGB(128, 128, 128));
            }
            for(int $1=0, $e=(0<=WW ? WW : (int)A.size()+WW); $1<$e; ++$1) gvText($1 % W, $1 / W, 0.3, "%d", A[$1]);
            gvTextLeft(W, 0, "t: %f", t);
            gvTextLeft(W, 1, "T: %f", T);
        }
        i = P[lrand49(P.size())];
        oldS = S[i];
        newS = C[i][oldS][lrand49(C[i][oldS].size())];
        oldJ = i + D[oldS];
        newJ = i + D[newS];
        dSc = 0;
        if(1 == R[oldJ]) {
            dSc -= A[oldJ];
        }
        if(0 == R[newJ]) {
            dSc += A[newJ];
        }
        if(drand49() < exp(dSc / T)) {
            S[i] = newS;
            --R[oldJ];
            ++R[newJ];
        }
    }
    $L2:;
    for(const int & i0 : range(WW)) {
        if(R[i0] != 0) {
            continue;
        }
        while(A[i0]) {
            i = i0;
            while(true) {
                if(!A[i]) {
                    goto $L5;
                }
                --A[i];
                print(i / W + 1, i % W + 1);
                if(!S[i]) {
                    goto $L5;
                }
                j = i + D[S[i]];
                if(A[i] != A[j]) {
                    goto $L5;
                }
                i = j;
            }
            $L5:;
        }
    }
    return 0;
}
