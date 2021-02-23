//#define NDEBUG
#ifdef mmlang_source_10_lines

K = inputInt()
X = 7 mod K
for i in range(K):
    if X==0:
        print(i+1)
        break
    X *= 10 mod K
    X += 7 mod K
else:
    print(-1)

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
}int inputInt(){return atoi(inputCWord());}inline int mod(int x,int p){if(p<=x){do{x-=p;}while(p<=x);}else if(x<0){do{x+=p;}while(x<0);}return x;}struct _range_ite{int i;int s;_range_ite(int i,int s):i(i),s(s){}bool operator !=(const _range_ite&o)const
{return i!=o.i;}void operator++(){i+=s;}const int&operator*()const{return i;}};struct _range_t{int b;int e;int s;_range_t(int b,int e,int s):b(b),e(e),s(s){}_range_ite begin(){return _range_ite(b,s);}_range_ite end(){return _range_ite(e,s);}};_range_t
range(int i){return _range_t(0,std::max(0,i),1);}_range_t range(int b,int e){return _range_t(b,std::max(b,e),1);}_range_t range(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)/-s+1)*s;}return _range_t(b,
e,s);}_range_t rrange(int i){return _range_t(std::max(0,i)-1,-1,-1);}_range_t rrange(int b,int e){return _range_t(std::max(b,e)-1,b-1,-1);}_range_t rrange(int b,int e,int s){assert(s!=0);if(0<s){e=b+(std::max(0,e-b-1)/ s+1)*s;}else{e=b+(std::max(0,b-e-1)
/-s+1)*s;}return _range_t(e-s,b-s,-s);}void print_unit(bool val){printf("%s",val?"true":"false");}void print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val){printf("%u",val);}void print_unit(size_t
val){printf("%zd",val);}void print_unit(const void*val){printf("%p",val);}void print_unit(long long val){printf("%lld",val);}void print_unit(__int128 val){char buf[128];int idx=128;buf[--idx]='\0';bool sign=false;if(val<0){sign=true;val=-val;}if(val==0)
{buf[--idx]='0';}else while(val){buf[--idx]='0'+(val % 10);val /=10;}if(sign){buf[--idx]='-';}printf("%s",buf+idx);}void print_unit(double val){printf("%g",val);}void print_unit(const char*val){printf("%s",val);}void print_unit(const std::string&val)
{printf("%s",val.c_str());}template<class T>void print_unit(const std::vector<T>&val){printf("vec(%d) {",(int)val.size());for(int i=0;i<(int)val.size();++i){fputc(' ',stdout);print_unit(val[i]);}printf(" }");}
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
inline int mod_mul(int x,int y,int p){__asm__ __volatile__("imul %%edx; idiv %2":"+a"(y),"+d"(x):"r"(p));if(x<0){x+=p;}return x;}

// generated code ( by mmlang ... https://github.com/colun/mmlang ) :

int K;
int X;

int main() {
    K = inputInt();
    X = mod(7, K);
    for(const int & i : range(K)) {
        if(X == 0) {
            print(i + 1);
            goto $L1;
        }
        X = mod_mul(X, 10, K);
        X = mod((X) + (7), K);
    }
    {
        print(-1);
    }
    $L1:;
    return 0;
}
