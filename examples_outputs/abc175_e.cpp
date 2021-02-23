//#define NDEBUG
#ifdef mmlang_source_24_lines

R, C, K = inputInt(), inputInt(), inputInt()
rcv[:K] = inputInt() - 1, inputInt() - 1, inputInt()
rcv.sort()
V[:R][:C] = 0
for r, c, v in rcv:
    V[r][c] = v
D[:R][:C][:4] = 0LL
for r in range(R):
    for c in range(C):
        #v = 0
        # v = (:rcv.where(r, c))[2]
        v = V[r][c]
        for i in range(4):
            if 1 <= i:
                D[r][c][i] = max(D[r][c][i], v)
            if 1 <= r:
                if 1 <= i:
                    D[r][c][i] = max(D[r][c][i], D[r - 1][c][:4] + v)
                else:
                    D[r][c][i] = max(D[r][c][i], D[r - 1][c][:4])
            if 1 <= c:
                D[r][c][i] = max(D[r][c][i], D[r][c - 1][:i] + v)
                D[r][c][i] = max(D[r][c][i], D[r][c - 1][:i + 1])
print(D[R - 1][C - 1][3])

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

int R;
int C;
int K;
vector<tuple<int, int, int> > rcv;
vector<vector<int> > V;
vector<vector<vector<long long> > > D;
int v;

int main() {
    {
        const auto & $c1 = inputInt();
        const auto & $c2 = inputInt();
        const auto & $c3 = inputInt();
        tie(R, C, K) = tuple<int, int, int>($c1, $c2, $c3);
    }
    {
        rcv.resize(max((int)rcv.size(), (int)K));
        for(int $1=0, $e=(0<=K ? K : (int)rcv.size()+K); $1<$e; ++$1) {
            const auto & $c1 = inputInt() - 1;
            const auto & $c2 = inputInt() - 1;
            const auto & $c3 = inputInt();
            rcv[$1] = tuple<int, int, int>($c1, $c2, $c3);
        }
    }
    sort(rcv.begin(), rcv.end());
    {
        V.resize(max((int)V.size(), (int)R));
        for(int $1=0, $e=(0<=R ? R : (int)V.size()+R); $1<$e; ++$1) {
            V[$1].resize(max((int)V[$1].size(), (int)C));
            for(int $2=0, $e=(0<=C ? C : (int)V[$1].size()+C); $2<$e; ++$2) V[$1][$2] = 0;
        }
    }
    for(const tuple<int, int, int> & $fr : rcv) { const int & r = get<0>($fr); const int & c = get<1>($fr); const int & v = get<2>($fr);
        V[r][c] = v;
    }
    {
        D.resize(max((int)D.size(), (int)R));
        for(int $1=0, $e=(0<=R ? R : (int)D.size()+R); $1<$e; ++$1) {
            D[$1].resize(max((int)D[$1].size(), (int)C));
            for(int $2=0, $e=(0<=C ? C : (int)D[$1].size()+C); $2<$e; ++$2) {
                D[$1][$2].resize(max((int)D[$1][$2].size(), (int)4));
                for(int $3=0, $e=(0<=4 ? 4 : (int)D[$1][$2].size()+4); $3<$e; ++$3) D[$1][$2][$3] = 0LL;
            }
        }
    }
    for(const int & r : range(R)) {
        for(const int & c : range(C)) {
            v = V[r][c];
            for(const int & i : range(4)) {
                if(1 <= i) {
                    D[r][c][i] = max((long long)(D[r][c][i]), (long long)(v));
                }
                if(1 <= r) {
                    if(1 <= i) {
                        for(int $1=0, $e=(0<=4 ? 4 : (int)D[r - 1][c].size()+4); $1<$e; ++$1) D[r][c][i] = max((long long)(D[r][c][i]), (long long)(D[r - 1][c][$1] + v));
                    }
                    else {
                        for(int $1=0, $e=(0<=4 ? 4 : (int)D[r - 1][c].size()+4); $1<$e; ++$1) D[r][c][i] = max((long long)(D[r][c][i]), (long long)(D[r - 1][c][$1]));
                    }
                }
                if(1 <= c) {
                    for(int $1=0, $e=(0<=i ? i : (int)D[r][c - 1].size()+i); $1<$e; ++$1) D[r][c][i] = max((long long)(D[r][c][i]), (long long)(D[r][c - 1][$1] + v));
                    for(int $e0=i + 1, $1=0, $e=(0<=$e0 ? $e0 : (int)D[r][c - 1].size()+$e0); $1<$e; ++$1) D[r][c][i] = max((long long)(D[r][c][i]), (long long)(D[r][c - 1][$1]));
                }
            }
        }
    }
    print(D[R - 1][C - 1][3]);
    return 0;
}
