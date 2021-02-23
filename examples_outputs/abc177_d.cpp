//#define NDEBUG
#ifdef mmlang_source_17_lines

N, M = inputInt(), inputInt()
uf = unionfind(N)
A[:M], B[$1] = inputInt() - 1, inputInt() - 1
uf.unite(A[:], B[$1])
C[:N] = uf.root($1)
C.sort()
before = -1
cnt = 0
ret = 0
for c in C:
    if c != before:
        before = c
        cnt = 1
    else:
        ++cnt
    ret >?= cnt
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

// library code :

typedef long long int64;typedef __int128 int128;const char*inputCLineOrWord(int mode){static char buf[2097152];static int bufLen=sizeof(buf);static int bufPos=sizeof(buf);static bool canReadFlag=true;static bool crFlag=false;static bool enterFlag=false;
if(canReadFlag&&(enterFlag?bufLen<=bufPos:(int)sizeof(buf)<=bufPos+bufPos)){if(0<bufLen-bufPos){memmove(buf,buf+bufPos,bufLen-bufPos);bufLen-=bufPos;}else{bufLen=0;}char*result=fgets(buf+bufLen,sizeof(buf)-bufLen,stdin);canReadFlag=(result!=NULL);if(result!=NULL)
{int n=strlen(result);enterFlag=(n!=(int)sizeof(buf)-1-bufLen||(1<=bufLen+n&&buf[bufLen+n-1]=='\n'));bufLen+=n;}bufPos=0;}if(mode==0){int pos=bufPos;while(true){char c=buf[pos];if(c==32){buf[pos++]='\0';break;}else if(c==10){if(crFlag){crFlag=false;if(bufPos==pos)
{pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret=buf+bufPos;bufPos=pos;while(true){char c=buf[bufPos];if(c==32||c==10||c==13){++bufPos;}else{break;}}return ret;
}else if(mode==1){int pos=bufPos;while(true){char c=buf[pos];if(c==10){if(crFlag){crFlag=false;if(bufPos==pos){pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret
=buf+bufPos;bufPos=pos;if(crFlag){while(true){char c=buf[bufPos];if(c==13){++bufPos;crFlag=false;break;}else{break;}}}return ret;}else if(mode==2){return bufLen<=bufPos?NULL:buf+bufPos;}assert(false);return NULL;}const char*inputCWord(){return inputCLineOrWord(0);
}int inputInt(){return atoi(inputCWord());}struct unionfind{private:int N;vector<int> parent;vector<int> count;public:unionfind():N(0),parent(),count(){}unionfind(int N):N(N),parent(N),count(N,1){for(int i=0,e=N;i<e;++i){parent[i]=i;}}unionfind(unionfind
&o):N(o.N),parent(o.parent),count(o.count){}void clear(){for(int i=0,e=N;i<e;++i){parent[i]=i;count[i]=1;}}int root(int i){if(parent[i]==i){return i;}return parent[i]=root(parent[i]);}void unite(int u,int v){u=root(u);v=root(v);if(u!=v){if(count[u]<count[v])
{parent[u]=v;count[v]+=count[u];}else{parent[v]=u;count[u]+=count[v];}}}};void print_unit(bool val){printf("%s",val?"true":"false");}void print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val)
{printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void print_unit(const void*val){printf("%p",val);}void print_unit(long long val){printf("%lld",val);}void print_unit(__int128 val){char buf[128];int idx=128;buf[--idx]='\0';bool sign=false;
if(val<0){sign=true;val=-val;}if(val==0){buf[--idx]='0';}else while(val){buf[--idx]='0'+(val % 10);val /=10;}if(sign){buf[--idx]='-';}printf("%s",buf+idx);}void print_unit(double val){printf("%g",val);}void print_unit(const char*val){printf("%s",val);
}void print_unit(const std::string&val){printf("%s",val.c_str());}template<class T>void print_unit(const std::vector<T>&val){printf("vec(%d) {",(int)val.size());for(int i=0;i<(int)val.size();++i){fputc(' ',stdout);print_unit(val[i]);}printf(" }");}
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
int M;
unionfind uf;
vector<int> A;
vector<int> B;
vector<int> C;
int before;
int cnt;
int ret;

int main() {
    {
        const auto & $c1 = inputInt();
        const auto & $c2 = inputInt();
        tie(N, M) = tuple<int, int>($c1, $c2);
    }
    uf = unionfind(N);
    {
        A.resize(max((int)A.size(), (int)M));
        B.resize(max((int)B.size(), (int)M));
        for(int $1=0, $e=(0<=M ? M : (int)A.size()+M); $1<$e; ++$1) {
            const auto & $c1 = inputInt() - 1;
            const auto & $c2 = inputInt() - 1;
            tie(A[$1], B[$1]) = tuple<int, int>($c1, $c2);
        }
    }
    for(int $1=0, $e=(int)A.size(); $1<$e; ++$1) uf.unite(A[$1], B[$1]);
    {
        C.resize(max((int)C.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)C.size()+N); $1<$e; ++$1) C[$1] = uf.root($1);
    }
    sort(C.begin(), C.end());
    before = -1;
    cnt = 0;
    ret = 0;
    for(const int & c : C) {
        if(c != before) {
            before = c;
            cnt = 1;
        }
        else {
            ++cnt;
        }
        ret = max(ret, cnt);
    }
    print(ret);
    return 0;
}
