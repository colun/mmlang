//#define NDEBUG
#ifdef mmlang_source_28_lines

N, K = inputInt(), inputInt()
P[:N] = inputInt()-1
C[:N] = inputInt()
ret = -100000000000LL
for start in range(N):
    sz = N
    i = start
    for j in range(N):
        i = P[i]
        if i==start:
            sz = j+1
            break
    K2 = K % sz
    CYCLE = K / sz
    MAX_SINGLE, MAX_CYCLE = -100000000000LL, -100000000000LL
    sum = 0LL
    for j in range(min(K, sz)):
        i = P[i]
        if i==start:
            sz = j
        sum += C[i]
        MAX_SINGLE = max(MAX_SINGLE, sum)
        if j<K2:
            MAX_CYCLE = max(MAX_CYCLE, sum)
    MAX_CYCLE += max(0LL, sum) * max(0, CYCLE)
    MAX_SINGLE += max(0LL, sum) * max(0, CYCLE-1)
    ret = max(ret, MAX_CYCLE, MAX_SINGLE)
print(ret)

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
_range_t(std::max(b,e)-1,b-1,-1);}_range_t rrange(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(e-s,b-s,-s);}void print_unit(bool val){printf("%s",val?"true":"false");}void
print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val){printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void print_unit(const void*val){printf("%p",val);}void print_unit(long
long val){printf("%lld",val);}void print_unit(__int128 val){char buf[128];int idx=128;buf[--idx]='\0';bool sign=false;if(val<0){sign=true;val=-val;}if(val==0){buf[--idx]='0';}else while(val){buf[--idx]='0'+(val % 10);val /=10;}if(sign){buf[--idx]='-';
}printf("%s",buf+idx);}void print_unit(double val){printf("%g",val);}void print_unit(const char*val){printf("%s",val);}void print_unit(const std::string&val){printf("%s",val.c_str());}template<class T>void print_unit(const std::vector<T>&val){printf("vec(%d) {"
,(int)val.size());for(int i=0;i<(int)val.size();++i){fputc(' ',stdout);print_unit(val[i]);}printf(" }");}
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

int N;
int K;
vector<int> P;
vector<int> C;
long long ret;
int sz;
int i;
int K2;
int CYCLE;
long long MAX_SINGLE;
long long MAX_CYCLE;
long long sum;

int main() {
    {
        const auto & $c1 = inputInt();
        const auto & $c2 = inputInt();
        tie(N, K) = tuple<int, int>($c1, $c2);
    }
    {
        P.resize(max((int)P.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)P.size()+N); $1<$e; ++$1) P[$1] = inputInt() - 1;
    }
    {
        C.resize(max((int)C.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)C.size()+N); $1<$e; ++$1) C[$1] = inputInt();
    }
    ret = -100000000000LL;
    for(const int & start : range(N)) {
        sz = N;
        i = start;
        for(const int & j : range(N)) {
            i = P[i];
            if(i == start) {
                sz = j + 1;
                goto $L2;
            }
        }
        $L2:;
        K2 = K % sz;
        CYCLE = K / sz;
        {
            const auto & $c1 = -100000000000LL;
            const auto & $c2 = -100000000000LL;
            tie(MAX_SINGLE, MAX_CYCLE) = tuple<long long, long long>($c1, $c2);
        }
        sum = 0LL;
        for(const int & j : range(min((int)(K), (int)(sz)))) {
            i = P[i];
            if(i == start) {
                sz = j;
            }
            sum += C[i];
            MAX_SINGLE = max((long long)(MAX_SINGLE), (long long)(sum));
            if(j < K2) {
                MAX_CYCLE = max((long long)(MAX_CYCLE), (long long)(sum));
            }
        }
        MAX_CYCLE += max((long long)(0LL), (long long)(sum)) * max((int)(0), (int)(CYCLE));
        MAX_SINGLE += max((long long)(0LL), (long long)(sum)) * max((int)(0), (int)(CYCLE - 1));
        ret = max(max((long long)(ret), (long long)(MAX_CYCLE)), (long long)(MAX_SINGLE));
    }
    print(ret);
    return 0;
}
