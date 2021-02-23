//#define NDEBUG
#ifdef mmlang_source_33_lines

N = inputInt()
S[:N], C[$1] = inputWord(), inputInt()
R[:N] = S[$1]
R[:N].reverse()
@memo(cycle=1e18LL)
def func(overIsPrefix: bool, over: string) -> int64:
    ret = 1e18LL
    for n in N:
        ret <?= func2(over, overIsPrefix ? S[n] : R[n], overIsPrefix) + C[n]
    return ret
def func2(a: string, b: string, flag: bool) -> int64:
    sum = 0
    sum += a[:min(a.size(), b.size())] != b[$1]
    if sum != 0:
        return 1e18LL
    elif a.size() == b.size():
        return 0
    elif b.size() < a.size():
        return func(flag, a.substr(b.size()))
    else:
        return func(!flag, b.substr(a.size()))
def solve() -> int64:
    ret = 1e18LL
    for n in N:
        s = S[n]
        for i in s.size()+1:
            prefix = s.substr(0, i)
            prefix.reverse()
            if i < s.size():
                ret <?= func2(prefix, s.substr(i + 1), True) + C[n]
            ret <?= func2(prefix, s.substr(i), True) + C[n]
    return ret==1e18LL ? -1 : ret
print(solve())

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
}int inputInt(){return atoi(inputCWord());}std::string inputWord(){return inputCWord();}void print_unit(bool val){printf("%s",val?"true":"false");}void print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned
int val){printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void print_unit(const void*val){printf("%p",val);}void print_unit(long long val){printf("%lld",val);}void print_unit(__int128 val){char buf[128];int idx=128;buf[--idx]='\0';bool
sign=false;if(val<0){sign=true;val=-val;}if(val==0){buf[--idx]='0';}else while(val){buf[--idx]='0'+(val % 10);val /=10;}if(sign){buf[--idx]='-';}printf("%s",buf+idx);}void print_unit(double val){printf("%g",val);}void print_unit(const char*val){printf("%s"
,val);}void print_unit(const std::string&val){printf("%s",val.c_str());}template<class T>void print_unit(const std::vector<T>&val){printf("vec(%d) {",(int)val.size());for(int i=0;i<(int)val.size();++i){fputc(' ',stdout);print_unit(val[i]);}printf(" }"
);}
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

// generated code ( by mmlang ... https://github.com/colun/mmlang ) :

int64 func(bool overIsPrefix, string over);
int64 func2(string a, string b, bool flag);
int64 solve();

int N;
vector<string> S;
vector<int> C;
vector<string> R;

int64 func$origin(bool overIsPrefix, string over) {
    long long ret;
    ret = (long long)1e18;
    for(int n=0; n<N; ++n) {
        ret = min(ret, (long long)(func2(over, overIsPrefix ? S[n] : R[n], overIsPrefix) + C[n]));
    }
    return ret;
}
map<tuple<bool, string>, int64> func$memo;
int64 func(bool overIsPrefix, string over) {
    auto it = func$memo.find(tuple<bool, string>(overIsPrefix, over));
    if(it!=func$memo.end()) {
        return it->second;
    }
    func$memo[tuple<bool, string>(overIsPrefix, over)] = (long long)1e18;
    auto ret = func$origin(overIsPrefix, over);
    func$memo[tuple<bool, string>(overIsPrefix, over)] = ret;
    return ret;
}

int64 func2(string a, string b, bool flag) {
    int sum;
    sum = 0;
    for(int $e0=min((int)(a.size()), (int)(b.size())), $1=0, $e=(0<=$e0 ? $e0 : (int)a.size()+$e0); $1<$e; ++$1) sum += a[$1] != b[$1];
    if(sum != 0) {
        return (long long)1e18;
    }
    else if(a.size() == b.size()) {
        return 0;
    }
    else if(b.size() < a.size()) {
        return func(flag, a.substr(b.size()));
    }
    else {
        return func(!flag, b.substr(a.size()));
    }
}

int64 solve() {
    long long ret;
    string s;
    string prefix;
    ret = (long long)1e18;
    for(int n=0; n<N; ++n) {
        s = S[n];
        for(int i=0; i<s.size() + 1; ++i) {
            prefix = s.substr(0, i);
            reverse(prefix.begin(), prefix.end());
            if(i < s.size()) {
                ret = min(ret, (long long)(func2(prefix, s.substr(i + 1), true) + C[n]));
            }
            ret = min(ret, (long long)(func2(prefix, s.substr(i), true) + C[n]));
        }
    }
    return ret == (long long)1e18 ? -1 : ret;
}

int main() {
    N = inputInt();
    {
        S.resize(max((int)S.size(), (int)N));
        C.resize(max((int)C.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)S.size()+N); $1<$e; ++$1) {
            const auto & $c1 = inputWord();
            const auto & $c2 = inputInt();
            tie(S[$1], C[$1]) = tuple<string, int>($c1, $c2);
        }
    }
    {
        R.resize(max((int)R.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)R.size()+N); $1<$e; ++$1) R[$1] = S[$1];
    }
    for(int $1=0, $e=(0<=N ? N : (int)R.size()+N); $1<$e; ++$1) reverse(R[$1].begin(), R[$1].end());
    print(solve());
    return 0;
}
