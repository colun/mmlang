//#define NDEBUG
#ifdef mmlang_source_21_lines

N, M = inputInt(), inputInt()
AB[:M] = inputInt() - 1, inputInt() - 1
AB[M:M * 2] = AB[$1 - M][1], AB[$1 - M][0]
AB.sort()
C[:N] = -1
C[0] = 0
F = queue!(int)()
F.push(0)
for f in F:
    for ab in AB.where(f):
        if C[t@ab[1]] == -1:
            C[t] = C[f] + 1
            F.push(t)
print("Yes")
for f in range(1, N):
    V[:0] = 0, 0
    V.clear()
    for ab in AB.where(f):
        V.emplace_back(C[t@ab[1]], t)
    V.sort()
    print(V[0][1] + 1)

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

// library code :

typedef long long int64;typedef __int128 int128;const char*inputCLineOrWord(int mode){static char buf[2097152];static int bufLen=sizeof(buf);static int bufPos=sizeof(buf);static bool canReadFlag=true;static bool crFlag=false;static bool enterFlag=false;
if(canReadFlag&&(enterFlag?bufLen<=bufPos:(int)sizeof(buf)<=bufPos+bufPos)){if(0<bufLen-bufPos){memmove(buf,buf+bufPos,bufLen-bufPos);bufLen-=bufPos;}else{bufLen=0;}char*result=fgets(buf+bufLen,sizeof(buf)-bufLen,stdin);canReadFlag=(result!=NULL);if(result!=NULL)
{int n=strlen(result);enterFlag=(n!=(int)sizeof(buf)-1-bufLen||(1<=bufLen+n&&buf[bufLen+n-1]=='\n'));bufLen+=n;}bufPos=0;}if(mode==0){int pos=bufPos;while(true){char c=buf[pos];if(c==32){buf[pos++]='\0';break;}else if(c==10){if(crFlag){crFlag=false;if(bufPos==pos)
{pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret=buf+bufPos;bufPos=pos;while(true){char c=buf[bufPos];if(c==32||c==10||c==13){++bufPos;}else{break;}}return ret;
}else if(mode==1){int pos=bufPos;while(true){char c=buf[pos];if(c==10){if(crFlag){crFlag=false;if(bufPos==pos){pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret
=buf+bufPos;bufPos=pos;if(crFlag){while(true){char c=buf[bufPos];if(c==13){++bufPos;crFlag=false;break;}else{break;}}}return ret;}else if(mode==2){return bufLen<=bufPos?NULL:buf+bufPos;}assert(false);return NULL;}const char*inputCWord(){return inputCLineOrWord(0);
}int inputInt(){return atoi(inputCWord());}template<class It> class _slice_t{private:It b;It e;public:_slice_t(It b,It e):b(b),e(e){}It begin(){return b;}It end(){return e;}};template<class It> _slice_t<It> _slice(It b,It e){return _slice_t<It>(b,e);
}void print_unit(bool val){printf("%s",val?"true":"false");}void print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val){printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void
print_unit(const void*val){printf("%p",val);}void print_unit(long long val){printf("%lld",val);}void print_unit(__int128 val){char buf[128];int idx=128;buf[--idx]='\0';bool sign=false;if(val<0){sign=true;val=-val;}if(val==0){buf[--idx]='0';}else while(val)
{buf[--idx]='0'+(val % 10);val /=10;}if(sign){buf[--idx]='-';}printf("%s",buf+idx);}void print_unit(double val){printf("%g",val);}void print_unit(const char*val){printf("%s",val);}void print_unit(const std::string&val){printf("%s",val.c_str());}template<class
T>void print_unit(const std::vector<T>&val){printf("vec(%d) {",(int)val.size());for(int i=0;i<(int)val.size();++i){fputc(' ',stdout);print_unit(val[i]);}printf(" }");}
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
struct _range_ite{int i;int s;_range_ite(int i,int s):i(i),s(s){}bool operator !=(const _range_ite&o)const{return i!=o.i;}void operator++(){i+=s;}const int&operator*()const{return i;}};struct _range_t{int b;int e;int s;_range_t(int b,int e,int s):b(b),
e(e),s(s){}_range_ite begin(){return _range_ite(b,s);}_range_ite end(){return _range_ite(e,s);}};_range_t range(int i){return _range_t(0,std::max(0,i),1);}_range_t range(int b,int e){return _range_t(b,std::max(b,e),1);}_range_t range(int b,int e,int s)
{assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(b,e,s);}_range_t rrange(int i){return _range_t(std::max(0,i)-1,-1,-1);}_range_t rrange(int b,int e){return _range_t(std::max(b,e)-1,b-1,-1);}_range_t
rrange(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(e-s,b-s,-s);}

// generated code ( by mmlang ... https://github.com/colun/mmlang ) :

int N;
int M;
vector<tuple<int, int> > AB;
vector<int> C;
queue<int> F;
vector<tuple<int, int> > V;

int main() {
    {
        const auto & $c1 = inputInt();
        const auto & $c2 = inputInt();
        tie(N, M) = tuple<int, int>($c1, $c2);
    }
    {
        AB.resize(max((int)AB.size(), (int)M));
        for(int $1=0, $e=(0<=M ? M : (int)AB.size()+M); $1<$e; ++$1) {
            const auto & $c1 = inputInt() - 1;
            const auto & $c2 = inputInt() - 1;
            AB[$1] = tuple<int, int>($c1, $c2);
        }
    }
    {
        AB.resize(max((int)AB.size(), (int)M * 2));
        for(int $e0=M * 2, $1=(0<=M ? M : (int)AB.size()+M), $e=(0<=$e0 ? $e0 : (int)AB.size()+$e0); $1<$e; ++$1) {
            const auto & $c1 = get<1>(AB[$1 - M]);
            const auto & $c2 = get<0>(AB[$1 - M]);
            AB[$1] = tuple<int, int>($c1, $c2);
        }
    }
    sort(AB.begin(), AB.end());
    {
        C.resize(max((int)C.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)C.size()+N); $1<$e; ++$1) C[$1] = -1;
    }
    C[0] = 0;
    F = queue<int>();
    F.push(0);
    while(!(F).empty()) { int f = (F).front(); (F).pop();
        for(const tuple<int, int> & ab : _slice(lower_bound(AB.begin(), AB.end(), tuple<int, int>(f, -2147483648)), upper_bound(AB.begin(), AB.end(), tuple<int, int>(f, 2147483647)))) {
            {
                const auto & t = get<1>(ab);
                if(C[t] == -1) {
                    C[t] = C[f] + 1;
                    F.push(t);
                }
            }
        }
    }
    print("Yes");
    for(const int & f : range(1, N)) {
        {
            V.resize(max((int)V.size(), (int)0));
            for(int $1=0, $e=(0<=0 ? 0 : (int)V.size()+0); $1<$e; ++$1) V[$1] = tuple<int, int>(0, 0);
        }
        V.clear();
        for(const tuple<int, int> & ab : _slice(lower_bound(AB.begin(), AB.end(), tuple<int, int>(f, -2147483648)), upper_bound(AB.begin(), AB.end(), tuple<int, int>(f, 2147483647)))) {
            {
                const auto & t = get<1>(ab);
                V.emplace_back(C[t], t);
            }
        }
        sort(V.begin(), V.end());
        print(get<1>(V[0]) + 1);
    }
    return 0;
}
