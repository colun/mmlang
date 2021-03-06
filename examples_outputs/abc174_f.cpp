//#define NDEBUG
#ifdef mmlang_source_14_lines

N, Q = inputInt(), inputInt()
c[:N] = inputInt() - 1
LR[:Q] = inputInt() - 1, inputInt() - 1, $1
LR.rsort()
D[:N] = 0
ans[:Q] = 0
bit, n = BIT!(int)(N), N - 1
for l, r, q in LR:
    while l <= n:
        bit[D[c[n]]] = 0
        bit[D[c[n]] = n] = 1
        --n
    ans[q] = bit.sum(l, r + 1)
print(ans[:Q])

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
}int inputInt(){return atoi(inputCWord());}template<class T> class BIT{private:std::vector<T> data;public:inline BIT(int n=0):data(n){assert(0<=n);}inline int size(){return(int)data.size();}inline void resize(int n){data.resize(n);}inline void clear()
{data.clear();}inline void add(int i,T o=1){assert(0<=i);assert(i<(int)data.size());if(o==0){return;}data[i]+=o;while(i){i&=i-1;data[i]+=o;}}inline T sum_back(int i)const{if(i){T ret=0;while(i<(int)data.size()){ret+=data[i];i+=i&-i;}return ret;}else{
return data[0];}}inline T sum(int i)const{assert(0<=i);assert(i<=(int)data.size());return data[0]-sum_back(i);}inline T sum(int l,int r)const{assert(0<=l);assert(l<=(int)data.size());assert(0<=r);assert(r<=(int)data.size());return sum_back(l)-sum_back(r);
}inline T get(int i)const{assert(0<=i);assert(i<(int)data.size());T ret=data[i];int j=1;int i2=~i;while(i2&j){int ij=i+j;if((int)data.size()<=ij){break;}ret-=data[ij];j+=j;}return ret;}class ref{private:BIT*bit;int i;public:inline ref(BIT*bit,int i):
bit(bit),i(i){}inline void operator=(const T&o){bit->add(i,o-bit->get(i));}inline void operator+=(const T&o){bit->add(i,o);}inline void operator-=(const T&o){bit->add(i,-o);}inline void operator*=(const T&o){bit->add(i,bit->get(i)*(o-1));}inline void
operator /=(const T&o){T v=bit->get(i);bit->add(i,v / o-v);}inline void operator++(){bit->add(i,1);}inline void operator++(int){bit->add(i,1);}inline void operator--(){bit->add(i,-1);}inline void operator--(int){bit->add(i,-1);}inline operator T()const
{return bit->get(i);}};inline ref operator[](int i){assert(0<=i);assert(i<(int)data.size());return ref(this,i);}inline T operator[](int i)const{assert(0<=i);assert(i<(int)data.size());return get(i);}};void print_unit(bool val){printf("%s",val?"true":"false"
);}void print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val){printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void print_unit(const void*val){printf("%p",val);}void print_unit(long
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

// generated code ( by mmlang ... https://github.com/colun/mmlang ) :

int N;
int Q;
vector<int> c;
vector<tuple<int, int, int> > LR;
vector<int> D;
vector<int> ans;
BIT<int> bit;
int n;

int main() {
    {
        const auto & $c1 = inputInt();
        const auto & $c2 = inputInt();
        tie(N, Q) = tuple<int, int>($c1, $c2);
    }
    {
        c.resize(max((int)c.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)c.size()+N); $1<$e; ++$1) c[$1] = inputInt() - 1;
    }
    {
        LR.resize(max((int)LR.size(), (int)Q));
        for(int $1=0, $e=(0<=Q ? Q : (int)LR.size()+Q); $1<$e; ++$1) {
            const auto & $c1 = inputInt() - 1;
            const auto & $c2 = inputInt() - 1;
            LR[$1] = tuple<int, int, int>($c1, $c2, $1);
        }
    }
    sort(LR.rbegin(), LR.rend());
    {
        D.resize(max((int)D.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)D.size()+N); $1<$e; ++$1) D[$1] = 0;
    }
    {
        ans.resize(max((int)ans.size(), (int)Q));
        for(int $1=0, $e=(0<=Q ? Q : (int)ans.size()+Q); $1<$e; ++$1) ans[$1] = 0;
    }
    {
        const auto & $c1 = BIT<int>(N);
        const auto & $c2 = N - 1;
        tie(bit, n) = tuple<BIT<int>, int>($c1, $c2);
    }
    for(const tuple<int, int, int> & $fr : LR) { const int & l = get<0>($fr); const int & r = get<1>($fr); const int & q = get<2>($fr);
        while(l <= n) {
            bit[D[c[n]]] = 0;
            bit[D[c[n]] = n] = 1;
            --n;
        }
        ans[q] = bit.sum(l, r + 1);
    }
    for(int $1=0, $e=(0<=Q ? Q : (int)ans.size()+Q); $1<$e; ++$1) print(ans[$1]);
    return 0;
}
