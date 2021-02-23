//#define NDEBUG
#ifdef mmlang_source_25_lines

N = inputInt()
B[:N] = 0, 0
for i in range(N):
    a, s = inputFraction64()
    B[i] = -s, -s
    while a % 2 == 0:
        a /= 2
        ++B[i][0]
    while a % 5 == 0:
        a /= 5
        ++B[i][1]
B.sort()
sum = 0LL
n = N - 1
bit = BIT!(int)(100)
for b2, b5 in B:
    assert -50<=b2<=50
    assert -50<=b5<=50
    while 0 <= n and 0 <= b2 + B[n][0]:
        ++bit[B[n][1] + 50]
        --n
    sum += bit.sum_back(50 - b5)
    sum -= 0 <= b2 and 0 <= b5
assert sum % 2 == 0
print(sum / 2)

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
}int inputInt(){return atoi(inputCWord());}struct _range_ite{int i;int s;_range_ite(int i,int s):i(i),s(s){}bool operator !=(const _range_ite&o)const{return i!=o.i;}void operator++(){i+=s;}const int&operator*()const{return i;}};struct _range_t{int b;
int e;int s;_range_t(int b,int e,int s):b(b),e(e),s(s){}_range_ite begin(){return _range_ite(b,s);}_range_ite end(){return _range_ite(e,s);}};_range_t range(int i){return _range_t(0,std::max(0,i),1);}_range_t range(int b,int e){return _range_t(b,std::max(b,
e),1);}_range_t range(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(b,e,s);}_range_t rrange(int i){return _range_t(std::max(0,i)-1,-1,-1);}_range_t rrange(int b,int e){return
_range_t(std::max(b,e)-1,b-1,-1);}_range_t rrange(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(e-s,b-s,-s);}tuple<long long,int> inputFraction64(){const char*p=inputCWord();
bool sign=false;if(*p=='-'){sign=true;++p;}long long ret=0;int scale=0;bool flag=false;while(true){if(*p=='.'){++p;flag=true;}else if('0'<=*p&&*p<='9'){ret=ret*10+(*p-'0');++p;if(flag){++scale;}}else{break;}}if(sign){ret=-ret;}return tuple<long long,
int>(ret,scale);}template<class T> class BIT{private:std::vector<T> data;public:inline BIT(int n=0):data(n){assert(0<=n);}inline int size(){return(int)data.size();}inline void resize(int n){data.resize(n);}inline void clear(){data.clear();}inline void
add(int i,T o=1){assert(0<=i);assert(i<(int)data.size());if(o==0){return;}data[i]+=o;while(i){i&=i-1;data[i]+=o;}}inline T sum_back(int i)const{if(i){T ret=0;while(i<(int)data.size()){ret+=data[i];i+=i&-i;}return ret;}else{return data[0];}}inline T sum(int
i)const{assert(0<=i);assert(i<=(int)data.size());return data[0]-sum_back(i);}inline T sum(int l,int r)const{assert(0<=l);assert(l<=(int)data.size());assert(0<=r);assert(r<=(int)data.size());return sum_back(l)-sum_back(r);}inline T get(int i)const{assert(0<=i);
assert(i<(int)data.size());T ret=data[i];int j=1;int i2=~i;while(i2&j){int ij=i+j;if((int)data.size()<=ij){break;}ret-=data[ij];j+=j;}return ret;}class ref{private:BIT*bit;int i;public:inline ref(BIT*bit,int i):bit(bit),i(i){}inline void operator=(const
T&o){bit->add(i,o-bit->get(i));}inline void operator+=(const T&o){bit->add(i,o);}inline void operator-=(const T&o){bit->add(i,-o);}inline void operator*=(const T&o){bit->add(i,bit->get(i)*(o-1));}inline void operator /=(const T&o){T v=bit->get(i);bit->add(i,
v / o-v);}inline void operator++(){bit->add(i,1);}inline void operator++(int){bit->add(i,1);}inline void operator--(){bit->add(i,-1);}inline void operator--(int){bit->add(i,-1);}inline operator T()const{return bit->get(i);}};inline ref operator[](int
i){assert(0<=i);assert(i<(int)data.size());return ref(this,i);}inline T operator[](int i)const{assert(0<=i);assert(i<(int)data.size());return get(i);}};void print_unit(bool val){printf("%s",val?"true":"false");}void print_unit(char val){printf("%c",val);
}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val){printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void print_unit(const void*val){printf("%p",val);}void print_unit(long long val){printf("%lld",val);}void print_unit(__int128
val){char buf[128];int idx=128;buf[--idx]='\0';bool sign=false;if(val<0){sign=true;val=-val;}if(val==0){buf[--idx]='0';}else while(val){buf[--idx]='0'+(val % 10);val /=10;}if(sign){buf[--idx]='-';}printf("%s",buf+idx);}void print_unit(double val){printf("%g"
,val);}void print_unit(const char*val){printf("%s",val);}void print_unit(const std::string&val){printf("%s",val.c_str());}template<class T>void print_unit(const std::vector<T>&val){printf("vec(%d) {",(int)val.size());for(int i=0;i<(int)val.size();++i)
{fputc(' ',stdout);print_unit(val[i]);}printf(" }");}
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
vector<tuple<int, int> > B;
long long a;
int s;
long long sum;
int n;
BIT<int> bit;

int main() {
    N = inputInt();
    {
        B.resize(max((int)B.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)B.size()+N); $1<$e; ++$1) B[$1] = tuple<int, int>(0, 0);
    }
    for(const int & i : range(N)) {
        tie(a, s) = inputFraction64();
        {
            const auto & $c1 = -s;
            const auto & $c2 = -s;
            B[i] = tuple<int, int>($c1, $c2);
        }
        while(a % 2 == 0) {
            a /= 2;
            ++get<0>(B[i]);
        }
        while(a % 5 == 0) {
            a /= 5;
            ++get<1>(B[i]);
        }
    }
    sort(B.begin(), B.end());
    sum = 0LL;
    n = N - 1;
    bit = BIT<int>(100);
    for(const tuple<int, int> & $fr : B) { const int & b2 = get<0>($fr); const int & b5 = get<1>($fr);
        assert((-50 <= b2 && b2 <= 50));
        assert((-50 <= b5 && b5 <= 50));
        while(0 <= n && 0 <= b2 + get<0>(B[n])) {
            ++bit[get<1>(B[n]) + 50];
            --n;
        }
        sum += bit.sum_back(50 - b5);
        sum -= 0 <= b2 && 0 <= b5;
    }
    assert(sum % 2 == 0);
    print(sum / 2);
    return 0;
}
