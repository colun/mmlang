//#define NDEBUG
#ifdef mmlang_source_85_lines

id, N, K = inputInt(), inputInt(), inputInt()
S[:N] = inputWord()
field[:10000] = S[$1/100][$1%100] - '1'
center = 4949

done: xarray!(char, 10000)
Q: array!(xvector!(int, 10000), 9)
answer: xvector!(int, 10000)
hash_seed: zobrist!(int, 10000)

done[:10000] = 0
best = vector!(int)()
best3 = 0

@beam(maxdepth=4, timelimit=2.9, maxwidth=1, verbose=false)
def search3(score, score0, color):
    s: stack!(int)
    for xy in Q[color]:
        s.push(xy)
    Q[color].clear()
    for xy in s:
        if done[xy]==2:
            continue
        c = field[xy]
        if c==color:
            x, y = xy % 100, xy / 100
            done[xy] = 2
            hash0 ^= hash_seed[xy]
            ++score0
            if x!=0:
                s.push(xy - 1)
            if x!=99:
                s.push(xy + 1)
            if y!=0:
                s.push(xy - 100)
            if y!=99:
                s.push(xy + 100)
        elif done[xy]==0:
            done[xy] = 1
            Q[c].push_back(xy)
    for c in 9:
        if !Q[c].empty():
            if best3<score0 + Q[c].size():
                best3 = score0 + Q[c].size()
            search3(score0 + Q[c].size(), score0, c)

@beam(maxdepth=170, timelimit=2.9, maxwidth=800)
def search2(hash, hash0, score, score0, color):
    answer.push_back(color)
    s: stack!(int)
    for xy in Q[color]:
        s.push(xy)
    Q[color].clear()
    for xy in s:
        if done[xy]==2:
            continue
        c = field[xy]
        if c==color:
            x, y = xy % 100, xy / 100
            done[xy] = 2
            hash0 ^= hash_seed[xy]
            ++score0
            if x!=0:
                s.push(xy - 1)
            if x!=99:
                s.push(xy + 1)
            if y!=0:
                s.push(xy - 100)
            if y!=99:
                s.push(xy + 100)
        elif done[xy]==0:
            done[xy] = 1
            Q[c].push_back(xy)
    if score0==10000:
        if best.empty() or answer.size() < best.size():
            best.assign(answer.begin(), answer.end())
    for c in 9:
        if !Q[c].empty():
            best3 = 0
            search3(score0 + Q[c].size(), score0, c)
            search2(hash0+c, hash0, best3, score0, c)
Q[field[center]].push_back(center)
search2(0, 0, 0, 0, field[center])
print(best.size()-1)
print(center/100+1, center%100+1, best[1:]+1)

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
#include <array>
#include <sys/time.h>
#include <cmath>
#include <utility>

using namespace std;

// library code :

typedef long long int64;typedef __int128 int128;const char*inputCLineOrWord(int mode){static char buf[2097152];static int bufLen=sizeof(buf);static int bufPos=sizeof(buf);static bool canReadFlag=true;static bool crFlag=false;static bool enterFlag=false;
if(canReadFlag&&(enterFlag?bufLen<=bufPos:(int)sizeof(buf)<=bufPos+bufPos)){if(0<bufLen-bufPos){memmove(buf,buf+bufPos,bufLen-bufPos);bufLen-=bufPos;}else{bufLen=0;}char*result=fgets(buf+bufLen,sizeof(buf)-bufLen,stdin);canReadFlag=(result!=NULL);if(result!=NULL)
{int n=strlen(result);enterFlag=(n!=(int)sizeof(buf)-1-bufLen||(1<=bufLen+n&&buf[bufLen+n-1]=='\n'));bufLen+=n;}bufPos=0;}if(mode==0){int pos=bufPos;while(true){char c=buf[pos];if(c==32){buf[pos++]='\0';break;}else if(c==10){if(crFlag){crFlag=false;if(bufPos==pos)
{pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret=buf+bufPos;bufPos=pos;while(true){char c=buf[bufPos];if(c==32||c==10||c==13){++bufPos;}else{break;}}return ret;
}else if(mode==1){int pos=bufPos;while(true){char c=buf[pos];if(c==10){if(crFlag){crFlag=false;if(bufPos==pos){pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret
=buf+bufPos;bufPos=pos;if(crFlag){while(true){char c=buf[bufPos];if(c==13){++bufPos;crFlag=false;break;}else{break;}}}return ret;}else if(mode==2){return bufLen<=bufPos?NULL:buf+bufPos;}assert(false);return NULL;}const char*inputCWord(){return inputCLineOrWord(0);
}int inputInt(){return atoi(inputCWord());}std::string inputWord(){return inputCWord();}
#define unlikely(expr)(__builtin_expect(!!(expr),0))
struct xmem{static char*base_ptr;static int buffer_size;static int redo_size;static std::vector<tuple<int,int> > locks;static char buffer[262144];std::vector<char*> mems;static constexpr int max_memsize=1048576-256;int used_mems;int mymemsize=0;char*
mymem=0;inline void clear(){used_mems=0;mymemsize=0;}inline xmem(){clear();}inline ~xmem(){}inline char*alloc(int size){if(mymemsize<size){assert(size<=max_memsize);if(mems.size()<=used_mems){mems.push_back((char*)std::malloc(max_memsize));}mymem=mems[used_mems++];
mymemsize=max_memsize;}char*ret=mymem;mymem+=size;mymemsize-=size;return ret;}static inline void free(void*patch){}static inline void lock(){locks.emplace_back(buffer_size,redo_size);redo_size=0;}static inline void unlock(){assert(!locks.empty());std::tie(buffer_size,
redo_size)=locks.back();locks.pop_back();}static inline bool enabled(){return !locks.empty();}static inline void init(void*mem){if unlikely(!base_ptr){base_ptr=(char*)mem;}}static inline void modify(void*mem,int size){if(locks.empty()){return;}assert(1<=size);
assert(size<65536);assert(buffer_size+size+6<=sizeof(buffer));std::memcpy(buffer+buffer_size,mem,size);buffer_size+=size;*(unsigned short*)(buffer+buffer_size)=size;buffer_size+=2;*(int*)(buffer+buffer_size)=((char*)mem)-base_ptr;buffer_size+=4;redo_size
+=size;}static inline void undo(){assert(!locks.empty());int base_buffer_size=std::get<0>(locks.back());while(base_buffer_size<buffer_size){buffer_size-=4;char*p=base_ptr+*(int*)(buffer+buffer_size);buffer_size-=2;int size=*(unsigned short*)(buffer+buffer_size);
buffer_size-=size;std::memcpy(p,buffer+buffer_size,size);}assert(base_buffer_size==buffer_size);redo_size=0;}inline void*build(){assert(!locks.empty());int base_buffer_size=std::get<0>(locks.back());int sz=(buffer_size-base_buffer_size)+redo_size+4;char
*ret=alloc(sz);*(unsigned int*)ret=sz;char*p=ret+4;while(base_buffer_size<buffer_size){buffer_size-=4;int pos=*(int*)(buffer+buffer_size);*(int*)p=pos;p+=4;buffer_size-=2;int size=*(unsigned short*)(buffer+buffer_size);*(unsigned short*)p=size;p+=2;std::memcpy(p,
base_ptr+pos,size);p+=size;buffer_size-=size;std::memcpy(p,buffer+buffer_size,size);p+=size;}assert(base_buffer_size==buffer_size);assert(p==ret+sz);redo_size=0;return ret;}static inline void undo(void*patch){assert(patch);char*p=(char*)patch;int sz=
*(unsigned int*)p;char*ep=p+sz;p+=4;while(p<ep){int pos=*(int*)p;p+=4;assert(p<=ep);int size=*(unsigned short*)p;p+=2+size;assert(p+size<=ep);std::memcpy(base_ptr+pos,p,size);p+=size;}assert(p==ep);}static inline void redo(void*patch){assert(patch);char
*p=(char*)patch;int sz=*(unsigned int*)p;char*ep=p+sz;p+=4;while(p!=ep){int pos=*(int*)p;p+=4;assert(p<=ep);int size=*(unsigned short*)p;p+=2;assert(p+size+size<=ep);std::memcpy(base_ptr+pos,p,size);p+=size+size;}assert(p==ep);}};char*xmem::base_ptr=
0;int xmem::buffer_size=0;int xmem::redo_size=0;std::vector<std::tuple<int,int> > xmem::locks;char xmem::buffer[262144];template<typename T,int N> class fast_array{private:char data_[sizeof(T)*N];public:inline T*data(){return(T*)data_;}inline const T*
data()const{return(const T*)data_;}inline std::size_t sizeofData()const{return sizeof(data_);}inline int size()const{return N;}inline T*begin(){return(T*)data_;}inline const T*begin()const{return(const T*)data_;}inline T*end(){return((T*)data_)+N;}inline
const T*end()const{return((const T*)data_)+N;}inline T&operator[](int i){assert(0<=i&&i<N);return((T*)data_)[i];}inline const T&operator[](int i)const{assert(0<=i&&i<N);return((const T*)data_)[i];}};template<class T> class xref{private:T*pointer;public:
inline xref(T*p):pointer(p){}inline void operator=(const T&o){if(*pointer!=o){xmem::modify(pointer,sizeof(T));*pointer=o;}}inline void operator+=(const T&o){T v=*pointer+o;if(*pointer!=v){xmem::modify(pointer,sizeof(T));*pointer=v;}}inline void operator
-=(const T&o){T v=*pointer-o;if(*pointer!=v){xmem::modify(pointer,sizeof(T));*pointer=v;}}inline void operator*=(const T&o){T v=*pointer*o;if(*pointer!=v){xmem::modify(pointer,sizeof(T));*pointer=v;}}inline void operator /=(const T&o){T v=*pointer / o;
if(*pointer!=v){xmem::modify(pointer,sizeof(T));*pointer=v;}}inline void operator&=(const T&o){T v=*pointer&o;if(*pointer!=v){xmem::modify(pointer,sizeof(T));*pointer=v;}}inline void operator|=(const T&o){T v=*pointer|o;if(*pointer!=v){xmem::modify(pointer,
sizeof(T));*pointer=v;}}inline void operator ^=(const T&o){T v=*pointer ^ o;if(*pointer!=v){xmem::modify(pointer,sizeof(T));*pointer=v;}}inline xref<T>&operator++(){xmem::modify(pointer,sizeof(T));++(*pointer);return*this;}inline T operator++(int){xmem::modify(pointer,
sizeof(T));T v=*pointer;++(*pointer);return v;}inline xref<T>&operator--(){xmem::modify(pointer,sizeof(T));--(*pointer);return*this;}inline T operator--(int){xmem::modify(pointer,sizeof(T));T v=*pointer;--(*pointer);return v;}inline T value()const{return
*pointer;}inline operator T()const{return*pointer;}};template<class T,int N> class xarray{private:fast_array<T,N> data_;public:inline xarray(){xmem::init(this);}inline void set(int i,const T&o){assert(0<=i&&i<N);xmem::modify(&data_[i],sizeof(data_[i]));
data_[i]=o;}inline const T&get(int i)const{assert(0<=i&&i<N);return data_[i];}inline const T*data()const{return data_.data();}inline int size()const{return N;}inline const T*begin()const{return data_.begin();}inline const T*end()const{return data_.end();
}inline xref<T> operator[](int i){assert(0<=i&&i<N);return xref<T>(&data_[i]);}inline const T&operator[](int i)const{assert(0<=i&&i<N);return data_[i];}};
#define likely(expr)(__builtin_expect(!!(expr),1))
template<typename T,int N> class xvector{private:int count;fast_array<T,N> data_;public:inline xvector():count(0){xmem::init(this);}inline void set(int i,const T&o){assert(0<=i&&i<count);xmem::modify(&data_[i],sizeof(data_[i]));data_[i]=o;}inline const
T&get(int i)const{assert(0<=i&&i<count);return data_[i];}inline void clear(){xmem::modify(&count,sizeof(count));count=0;}inline bool empty()const{return count==0;}inline bool full()const{return count==N;}inline int full_size()const{return N;}template<class
TyIt> inline void assign(TyIt st,TyIt et){int cnt=0;if(xmem::enabled()){TyIt it=st;while(it!=et){assert(cnt<N);++it;++cnt;}xmem::modify(&count,sizeof(count));xmem::modify(data_.data(),sizeof(T)*cnt);cnt=0;}while(st!=et){assert(cnt<N);data_[cnt]=*st;++st;
++cnt;}count=cnt;}inline void push_back(const T&value){assert(0<=count&&count<N);xmem::modify(&count,sizeof(count));xmem::modify(&data_[count],sizeof(T));data_[count]=value;++count;}template<class...TyArgs> inline void emplace_back(TyArgs&&...args){assert(0<=count
&&count<N);xmem::modify(&count,sizeof(count));xmem::modify(&data_[count],sizeof(T));::new(&data_[count])T(std::forward<TyArgs>(args)...);++count;}inline xref<T> let_back(){assert(0<=count&&count<N);xmem::modify(&count,sizeof(count));return xref<T>(data_
+(count++));}inline void pop_back(){assert(0<count);xmem::modify(&count,sizeof(count));--count;}inline const T&pop_back_with_value(){assert(0<count);xmem::modify(&count,sizeof(count));--count;return data_[count];}inline void pop_swap_back(int i){assert(0<=i
&&i<count);xmem::modify(&count,sizeof(count));--count;if likely(i!=count){xmem::modify(&data_[i],sizeof(data_[i]));data_[i]=data_[count];}}inline T pop_swap_back_with_value(int i){assert(0<=i&&i<count);xmem::modify(&count,sizeof(count));--count;T ret
=data_[i];if likely(i!=count){xmem::modify(&data_[i],sizeof(data_[i]));data_[i]=data_[count];}return ret;}inline xref<T> front(){assert(0<count);return xref<T>(data_);}inline const T&front()const{assert(0<count);return data_[0];}inline xref<T> back()
{assert(0<count);return xref<T>(data_+(count-1));}inline const T&back()const{assert(0<count);return data_[count-1];}inline int size()const{return count;}inline void resize(int n,const T&value){assert(0<=n&&n<=N);xmem::modify(&count,sizeof(count));if(count<n)
{xmem::modify(&data_[count],sizeof(*data_)*(n-count));for(int i=count;i<n;++i){data_[i]=value;}}count=n;}inline void resize(int n){assert(0<=n&&n<=N);xmem::modify(&count,sizeof(count));count=n;}inline const T*data()const{return data_.data();}inline const
T*begin()const{return data_.data();}inline const T*end()const{return data_.data()+count;}inline xref<T> operator[](int i){assert(0<=i&&i<count);return xref<T>(data_.data()+i);}inline const T&operator[](int i)const{assert(0<=i&&i<count);return data_[i];
}bool in_array_slow(const T&value)const{for(int i=0;i<count;++i){if unlikely(data_[i]==value){return true;}}return false;}};unsigned long long mrand49$state=0x8a5cd789635d2dffULL;inline int mrand49(){mrand49$state*=6364136223846793005ULL;mrand49$state
+=1442695040888963407ULL;unsigned int ret=((mrand49$state>>18)^mrand49$state)>> 27;unsigned int rot=(mrand49$state>>59);return(ret>>rot)|(ret<<-rot);}inline long long mmrand49(){int v=mrand49();return((long long)v<<32)|mrand49();}template<typename T,
int N> class zobrist{private:T hash[N];public:zobrist(){if(8<=sizeof(T)){for(int i=0;i<N;++i){hash[i]=(T)mmrand49();}}else{for(int i=0;i<N;++i){hash[i]=(T)mrand49();}}}inline const T&operator[](int i)const{assert(0<=i&&i<N);return hash[i];}};void print_unit(bool
val){printf("%s",val?"true":"false");}void print_unit(char val){printf("%c",val);}void print_unit(int val){printf("%d",val);}void print_unit(unsigned int val){printf("%u",val);}void print_unit(size_t val){printf("%zd",val);}void print_unit(const void
*val){printf("%p",val);}void print_unit(long long val){printf("%lld",val);}void print_unit(__int128 val){char buf[128];int idx=128;buf[--idx]='\0';bool sign=false;if(val<0){sign=true;val=-val;}if(val==0){buf[--idx]='0';}else while(val){buf[--idx]='0'
+(val % 10);val /=10;}if(sign){buf[--idx]='-';}printf("%s",buf+idx);}void print_unit(double val){printf("%g",val);}void print_unit(const char*val){printf("%s",val);}void print_unit(const std::string&val){printf("%s",val.c_str());}template<class T>void
print_unit(const std::vector<T>&val){printf("vec(%d) {",(int)val.size());for(int i=0;i<(int)val.size();++i){fputc(' ',stdout);print_unit(val[i]);}printf(" }");}
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
inline void getCpuClock(unsigned long long&t){__asm__ __volatile__("rdtsc":"=a"(*(unsigned int*)&t),"=d"(((unsigned int*)&t)[1]));}inline double getNativeTime(){timeval tv;gettimeofday(&tv,0);return tv.tv_sec+tv.tv_usec*1e-6;}unsigned long long getTime$initCpuClock;
unsigned long long getTime$reserveUpdateCpuClock;double getTime$initNativeTime;double getTime$secPerClock;double getTime$doneTime;struct getTime$init_class{inline getTime$init_class(){getTime$initNativeTime=getNativeTime();getCpuClock(getTime$initCpuClock);
getTime$secPerClock=0.00000000025;getTime$reserveUpdateCpuClock=10000000;getTime$doneTime=0;}};getTime$init_class getTime$init_obj;inline double getTime(){unsigned long long now;getCpuClock(now);now-=getTime$initCpuClock;if(getTime$reserveUpdateCpuClock
<now){double nowTime=getNativeTime()-getTime$initNativeTime;getTime$secPerClock=nowTime / now;getTime$reserveUpdateCpuClock=now+(unsigned long long)(0.05*now / nowTime);}return getTime$doneTime=std::fmax(getTime$doneTime,getTime$secPerClock*now);}template<typename
Ty,int blockSize=524288,int align=sizeof(void*)> class fast_pool{void*top;std::size_t mem;std::size_t it;std::size_t et;std::size_t unitSize;int unitSizeNum;public:int count;inline void init(std::size_t size=sizeof(Ty)){count=0;while(mem){std::size_t
m=*(std::size_t*)mem;std::free((void*)mem);mem=m;}top=NULL;it=0;et=0;unitSize=((size+(align-1))&~(align-1));int unitSizeInt=unitSize;int num=(blockSize-(sizeof(void*)+(align-1)))/ unitSize;unitSizeNum=unitSize*num;}inline fast_pool(){mem=0;init();}inline
void push(Ty*p){--count;*(void**)p=top;top=(void*)p;}inline Ty*pop(){++count;if(top!=NULL){Ty*result=(Ty*)top;top=*(void**)top;return result;}if(it==et){it=(std::size_t)std::malloc(unitSizeNum+(sizeof(void*)+(align-1)));*(std::size_t*)it=mem;mem=it;it
=(it+(sizeof(void*)+(align-1)))&~(align-1);et=it+unitSizeNum;}Ty*result=(Ty*)it;it+=unitSize;return result;}};template<typename T> struct dpque_dummy_callback{inline dpque_dummy_callback(){}inline void operator()(const T&o,int pos){}};template<typename
T,typename F=dpque_dummy_callback<T> > class dpque{private:F callback;std::vector<T> data;inline void adjustDown(int now,const T&value){int count=size();while(true){int nxt1=now+now+2;int nxt2=nxt1+1;if(nxt2<count){if(data[nxt1]<data[nxt2]){if(value<
data[nxt2]){data[now]=std::move(data[nxt2]);callback(data[now],now);now=nxt2;}else{data[now]=std::move(value);callback(data[now],now);return;}}else{if(value<data[nxt1]){data[now]=std::move(data[nxt1]);callback(data[now],now);now=nxt1;}else{data[now]=
std::move(value);callback(data[now],now);return;}}}else if(nxt1<count){if(value<data[nxt1]){data[now]=std::move(data[nxt1]);callback(data[now],now);now=nxt1;}else{data[now]=std::move(value);callback(data[now],now);return;}}else{break;}}int clz=__builtin_clz(now+2);
int nxt=now+(1<<(30-clz));if(count<=nxt){nxt=(nxt-2)>>1;}if(0<=nxt){while(value<data[nxt]){data[now]=std::move(data[nxt]);callback(data[now],now);now=nxt;if(now<2){break;}nxt=(now-2)>>1;}}data[now]=std::move(value);callback(data[now],now);}inline void
adjustUp(int now,const T&value){int count=size();while(true){int nxt1=now+now+2;int nxt2=nxt1+1;if(nxt2<count){if(data[nxt2]<data[nxt1]){if(data[nxt2]<value){data[now]=std::move(data[nxt2]);callback(data[now],now);now=nxt2;}else{data[now]=std::move(value);
callback(data[now],now);return;}}else{if(data[nxt1]<value){data[now]=std::move(data[nxt1]);callback(data[now],now);now=nxt1;}else{data[now]=std::move(value);callback(data[now],now);return;}}}else if(nxt1<count){if(data[nxt1]<value){data[now]=std::move(data[nxt1]);
callback(data[now],now);now=nxt1;}else{data[now]=std::move(value);callback(data[now],now);return;}}else{break;}}int clz=__builtin_clz(now+2);int nxt=now+now-(1<<(31-clz))+2;if(count<=nxt){nxt=(nxt-2)>>1;}if(0<=nxt){while(data[nxt]<value){data[now]=std::move(data[nxt]);
callback(data[now],now);now=nxt;if(now<2){break;}nxt=(now-2)>>1;}}data[now]=std::move(value);callback(data[now],now);}public:inline dpque(){}inline void clear(){for(T&o:data){callback(o,-1);}data.clear();}inline ~dpque(){clear();}inline void clearWithoutCallback()
{data.clear();}inline bool empty()const{return data.empty();}inline void push(const T&value){int now=size();if(2<=now){int clz=__builtin_clz(now+2);int nxt=(now-2)>>1;data.emplace_back();if(now<((3<<(30-clz))-2)){if(data[nxt]<value){do{data[now]=std::move(data[nxt]);
callback(data[now],now);now=nxt;if(now<2){break;}nxt=(now-2)>>1;}while(data[nxt]<value);}else{int nxt=(now+(1<<(30-clz))-2)>>1;while(value<data[nxt]){data[now]=std::move(data[nxt]);callback(data[now],now);now=nxt;if(now<2){break;}nxt=(now-2)>>1;}}}else
{if(value<data[nxt]){do{data[now]=std::move(data[nxt]);callback(data[now],now);now=nxt;if(now<2){break;}nxt=(now-2)>>1;}while(value<data[nxt]);}else{int nxt=now-(1<<(30-clz));while(data[nxt]<value){data[now]=std::move(data[nxt]);callback(data[now],now);
now=nxt;if(now<2){break;}nxt=(now-2)>>1;}}}data[now]=std::move(value);callback(data[now],now);}else if(now==1){if(data[0]<value){data.emplace_back(data[0]);callback(data[1],1);data[0]=std::move(value);callback(data[0],0);}else{data.emplace_back(std::move(value));
callback(data[1],1);}}else{assert(empty());data.push_back(std::move(value));callback(data[0],0);}}inline const T&large()const{assert(!empty());return data[0];}inline const T&small()const{assert(!empty());return size()==1?data[0]:data[1];}inline void pop_large()
{assert(!empty());if(size()==1){callback(data[0],-1);data.pop_back();}else{T o=std::move(data.back());data.pop_back();callback(data[0],-1);adjustDown(0,o);}}inline void pop_small(){assert(!empty());if(size()==1){callback(data[0],-1);data.pop_back();}
else{callback(data[1],-1);if(size()==2){data.pop_back();}else{T o=std::move(data.back());data.pop_back();adjustUp(1,o);}}}inline void swap_large(const T&value){assert(!empty());callback(data[0],-1);adjustDown(0,value);}inline void swap_small(const T&
value){assert(!empty());if(2<=size()){callback(data[1],-1);adjustUp(1,value);}else{callback(data[0],-1);data[0]=std::move(value);callback(data[0],0);}}inline void push_large(const T&value,int size_){if(size()<size_){push(value);}else if(small()<value)
{swap_small(value);}}inline void push_small(const T&value,int size_){if(size()<size_){push(value);}else if(value<large()){swap_large(value);}}template<class...TyArgs> inline void emplace(TyArgs&&...args){T value(std::forward<TyArgs>(args)...);push(value);
}template<class...TyArgs> inline void emplace_large(TyArgs&&...args){T value(std::forward<TyArgs>(args)...);push_large(value);}template<class...TyArgs> inline void emplace_small(TyArgs&&...args){T value(std::forward<TyArgs>(args)...);push_small(value);
}inline void swap(int now,const T&value){assert(0<=now&&now<size());callback(data[now],-1);int clz0=__builtin_clz(now+2);if(now<((3<<(30-clz0))-2)){if(2<=now){int nxt=(now-2)>>1;if(data[nxt]<value){do{data[now]=std::move(data[nxt]);callback(data[now],
now);now=nxt;if(now<2){break;}nxt=(now-2)>>1;}while(data[nxt]<value);data[now]=std::move(value);callback(data[now],now);return;}}adjustDown(now,value);}else{if(2<=now){int nxt=(now-2)>>1;if(value<data[nxt]){do{data[now]=std::move(data[nxt]);callback(data[now],
now);now=nxt;if(now<2){break;}nxt=(now-2)>>1;}while(value<data[nxt]);data[now]=std::move(value);callback(data[now],now);return;}}adjustUp(now,value);}}inline void remove(int i){assert(0<=i&&i<size());if(i+1==size()){callback(data[i],-1);data.pop_back();
}else{T o=std::move(data.back());data.pop_back();swap(i,o);}}inline const T&operator[](int i)const{assert(0<=i&&i<size());return data[i];}inline const T&get(int i)const{assert(0<=i&&i<size());return data[i];}inline int size()const{return data.size();
}typedef typename std::vector<T>::const_iterator const_iterator;inline const_iterator begin()const{return data.begin();}inline const_iterator end()const{return data.end();}};struct xbeam$node{static fast_pool<xbeam$node> pool;xbeam$node*parent;char*input;
void*patch;int depth;int ref;void addRef(){++ref;}void release(){assert(0<ref);if(ref==1){if(parent){parent->release();}xmem::free(patch);pool.push(this);}else{--ref;}}static xbeam$node*create(){xbeam$node*n=pool.pop();n->parent=0;n->input=0;n->patch
=0;n->depth=0;n->ref=1;return n;}static xbeam$node*create(xbeam$node*parent_,char*input_){parent_->addRef();xbeam$node*n=pool.pop();n->parent=parent_;n->input=input_;n->patch=0;n->depth=parent_->depth+1;n->ref=1;return n;}};fast_pool<xbeam$node> xbeam$node::pool;
struct xbeam{xmem myxmem;xbeam$node*current_node;xbeam$node*next_node=NULL;typedef std::pair<double,xbeam$node*> ranking_item;typedef dpque<ranking_item> ranking;ranking*current_ranking=NULL;ranking*next_ranking=NULL;char*input_mem=0;char*mymem=0;int
mymemsize=0;std::vector<char*> mymem_used;std::vector<char*> mymem_unused;std::vector<char*> mymem_current;double nextLimit;double wholeLimit;int remain_depth;int max_beam_size;int infoAcceptCount;int infoDepthAcceptCount;int infoLoopCount;int infoDepthLoopCount;
int infoDepth;double infoStartTime;double infoDepthStartTime;bool verboseFlag=true;xbeam(){}void setVerbose(bool flag){verboseFlag=flag;}void clear_ranking(ranking*rnk){for(const ranking_item&v:*rnk){v.second->release();}rnk->clear();}void init(int max_depth_,
double timeLimit=9.8,int max_beam_size_=2000){infoAcceptCount=0;infoDepthAcceptCount=0;infoLoopCount=0;infoDepthLoopCount=0;infoDepth=0;remain_depth=max_depth_;max_beam_size=max_beam_size_;wholeLimit=timeLimit;{double t=getTime();nextLimit=(wholeLimit
-t)/ remain_depth+t;infoDepthStartTime=infoStartTime=t;}current_node=xbeam$node::create();if(current_ranking){clear_ranking(current_ranking);}else{current_ranking=new ranking();}if(next_ranking){clear_ranking(next_ranking);}else{next_ranking=new ranking();
}xmem::lock();}void dest_(double t){xmem::unlock();if(verboseFlag){fprintf(stderr,"xbeam(all): %.3f->%.3f(%.6fs) %d/%d ... %.1fa/s\n\n",infoStartTime,t,t-infoStartTime,infoAcceptCount,infoLoopCount,infoAcceptCount/(t-infoStartTime));}xbeam$node*cn=current_node;
while(current_node){if(current_node->patch){xmem::undo(current_node->patch);}current_node=current_node->parent;}cn->release();}void infoForNextDepth_(double t){if(verboseFlag&&1<=infoLoopCount-infoDepthLoopCount){fprintf(stderr,"xbeam(%3d): %.3f->%.3f(%.6fs) %d/%d ... %.1fa/s\n"
,infoDepth,infoDepthStartTime,t,t-infoDepthStartTime,infoAcceptCount-infoDepthAcceptCount,infoLoopCount-infoDepthLoopCount,(infoAcceptCount-infoDepthAcceptCount)/(t-infoDepthStartTime));}infoDepthStartTime=t;infoDepthAcceptCount=infoAcceptCount;infoDepthLoopCount
=infoLoopCount;++infoDepth;}void transit_(xbeam$node*current_node,xbeam$node*next_node){assert(next_node);assert(next_node->patch==0);assert(next_node->parent!=0);next_node=next_node->parent;std::vector<void*> vec;while(current_node!=next_node){assert(current_node);
assert(next_node);if(current_node->depth<next_node->depth){assert(next_node->patch);vec.emplace_back(next_node->patch);next_node=next_node->parent;}else{assert(current_node->patch);xmem::undo(current_node->patch);current_node=current_node->parent;}}while(!vec.empty())
{xmem::redo(vec.back());vec.pop_back();}}void accept(){assert(next_node);++infoAcceptCount;transit_(current_node,next_node);current_node->release();current_node=next_node;next_node=NULL;}bool onloop(){if(next_node){next_node->release();next_node=NULL;
}else{current_node->patch=myxmem.build();}double t=getTime();if(nextLimit<=t||current_ranking->empty()){infoForNextDepth_(t);if(remain_depth<=0){dest_(t);return false;}nextLimit=(wholeLimit-t)/ remain_depth+t;--remain_depth;clear_ranking(current_ranking);
std::swap(current_ranking,next_ranking);for(char*p:mymem_current){mymem_unused.emplace_back(p);}mymem_current.swap(mymem_used);mymem_used.clear();mymemsize=0;}if(current_ranking->empty()){dest_(t);return false;}next_node=current_ranking->large().second;
current_ranking->pop_large();input_mem=next_node->input;++infoLoopCount;return true;}bool reserve(double score=0){if(max_beam_size<=next_ranking->size()){if(score<=next_ranking->small().first){return false;}next_ranking->small().second->release();next_ranking->pop_small();
}if(mymemsize<4096){mymemsize=1048576-256;if(mymem_unused.empty()){mymem=(char*)std::malloc(mymemsize);}else{mymem=mymem_unused.back();mymem_unused.pop_back();}mymem_used.emplace_back(mymem);}next_ranking->emplace(score,xbeam$node::create(current_node,
mymem));return true;}void nextDouble(double value){mymemsize-=sizeof(value);assert(0<=mymemsize);*(double*)mymem=value;mymem+=sizeof(value);}void nextFloat(float value){mymemsize-=sizeof(value);assert(0<=mymemsize);*(float*)mymem=value;mymem+=sizeof(value);
}void nextInt64(long long value){mymemsize-=sizeof(value);assert(0<=mymemsize);*(long long*)mymem=value;mymem+=sizeof(value);}void nextInt(int value){mymemsize-=sizeof(value);assert(0<=mymemsize);*(int*)mymem=value;mymem+=sizeof(value);}void nextShort(short
value){mymemsize-=sizeof(value);assert(0<=mymemsize);*(short*)mymem=value;mymem+=sizeof(value);}void nextChar(char value){mymemsize-=sizeof(value);assert(0<=mymemsize);*(char*)mymem=value;mymem+=sizeof(value);}double nextDouble(){double ret=*(double*)input_mem;
input_mem+=sizeof(ret);return ret;}float nextFloat(){float ret=*(float*)input_mem;input_mem+=sizeof(ret);return ret;}long long nextInt64(){long long ret=*(long long*)input_mem;input_mem+=sizeof(ret);return ret;}int nextInt(){int ret=*(int*)input_mem;
input_mem+=sizeof(ret);return ret;}short nextShort(){short ret=*(short*)input_mem;input_mem+=sizeof(ret);return ret;}char nextChar(){char ret=*(char*)input_mem;input_mem+=sizeof(ret);return ret;}};template<int N=65536>class fast_weak_set32{private:const
int mask=N-1;fast_array<unsigned short,N> memo;public:inline void clear(){memset(memo.data(),0,sizeof(memo));}inline fast_weak_set32(){assert((N&(N-1))==0);}inline bool test(int hash)const{int key=hash&mask;unsigned short hi=hash >> 16;if(hi==0){hi=1;
}return memo[key]==hi;}inline void set(long long hash){int key=hash&mask;unsigned short hi=hash >> 16;if(hi==0){hi=1;}memo[key]=hi;}inline void reset(long long hash){int key=hash&mask;memo[key]=0;}inline bool testWithSet(long long hash){int key=hash&
mask;unsigned short hi=hash >> 16;if(hi==0){hi=1;}if(memo[key]==hi){return true;}memo[key]=hi;return false;}};

// generated code ( by mmlang ... https://github.com/colun/mmlang ) :

void search3(int score, int score0, int color);
void search2(int hash, int hash0, int score, int score0, int color);

int id;
int N;
int K;
vector<string> S;
vector<char> field;
int center;
xarray<char, 10000> done;
array<xvector<int, 10000>, 9> Q;
xvector<int, 10000> answer;
zobrist<int, 10000> hash_seed;
vector<int> best;
int best3;

void search3$origin(int score, int score0, int color) {
    stack<int> s;
    char c;
    int x;
    int y;
    int hash0;
    for(const auto & xy : Q[color]) {
        s.push(xy);
    }
    Q[color].clear();
    while(!(s).empty()) { int xy = (s).top(); (s).pop();
        if(done[xy] == 2) {
            continue;
        }
        c = field[xy];
        if(c == color) {
            {
                const auto & $c1 = xy % 100;
                const auto & $c2 = xy / 100;
                tie(x, y) = tuple<int, int>($c1, $c2);
            }
            done[xy] = 2;
            hash0 ^= hash_seed[xy];
            ++score0;
            if(x != 0) {
                s.push(xy - 1);
            }
            if(x != 99) {
                s.push(xy + 1);
            }
            if(y != 0) {
                s.push(xy - 100);
            }
            if(y != 99) {
                s.push(xy + 100);
            }
        }
        else if(done[xy] == 0) {
            done[xy] = 1;
            Q[c].push_back(xy);
        }
    }
    for(int c=0; c<9; ++c) {
        if(!Q[c].empty()) {
            if(best3 < score0 + Q[c].size()) {
                best3 = score0 + Q[c].size();
            }
            search3(score0 + Q[c].size(), score0, c);
        }
    }
}
xbeam search3$so;
void search3$loop() {
    while(search3$so.onloop()) {
        auto score = search3$so.nextInt();
        auto score0 = search3$so.nextInt();
        auto color = search3$so.nextInt();
        search3$so.accept();
        search3$origin(score, score0, color);
    }
}
bool search3$lock = false;
void search3(int score, int score0, int color) {
    if(search3$lock) {
        if(search3$so.reserve(score)) {
            search3$so.nextInt(score);
            search3$so.nextInt(score0);
            search3$so.nextInt(color);
        }
    }
    else {
        search3$lock = true;
        search3$so.init((4)+1, 2.9, 1);
        search3$so.setVerbose(false);
        if(search3$so.reserve(score)) {
            search3$so.nextInt(score);
            search3$so.nextInt(score0);
            search3$so.nextInt(color);
        }
        search3$loop();
        search3$lock = false;
    }
}

void search2$origin(int hash, int hash0, int score, int score0, int color) {
    stack<int> s;
    char c;
    int x;
    int y;
    answer.push_back(color);
    for(const auto & xy : Q[color]) {
        s.push(xy);
    }
    Q[color].clear();
    while(!(s).empty()) { int xy = (s).top(); (s).pop();
        if(done[xy] == 2) {
            continue;
        }
        c = field[xy];
        if(c == color) {
            {
                const auto & $c1 = xy % 100;
                const auto & $c2 = xy / 100;
                tie(x, y) = tuple<int, int>($c1, $c2);
            }
            done[xy] = 2;
            hash0 ^= hash_seed[xy];
            ++score0;
            if(x != 0) {
                s.push(xy - 1);
            }
            if(x != 99) {
                s.push(xy + 1);
            }
            if(y != 0) {
                s.push(xy - 100);
            }
            if(y != 99) {
                s.push(xy + 100);
            }
        }
        else if(done[xy] == 0) {
            done[xy] = 1;
            Q[c].push_back(xy);
        }
    }
    if(score0 == 10000) {
        if(best.empty() || answer.size() < best.size()) {
            best.assign(answer.begin(), answer.end());
        }
    }
    for(int c=0; c<9; ++c) {
        if(!Q[c].empty()) {
            best3 = 0;
            search3(score0 + Q[c].size(), score0, c);
            search2(hash0 + c, hash0, best3, score0, c);
        }
    }
}
xbeam search2$so;
fast_weak_set32<> search2$visited;
void search2$loop() {
    while(search2$so.onloop()) {
        auto hash = search2$so.nextInt();
        if(search2$visited.testWithSet(hash)) continue;
        auto hash0 = search2$so.nextInt();
        auto score = search2$so.nextInt();
        auto score0 = search2$so.nextInt();
        auto color = search2$so.nextInt();
        search2$so.accept();
        search2$origin(hash, hash0, score, score0, color);
    }
}
bool search2$lock = false;
void search2(int hash, int hash0, int score, int score0, int color) {
    if(search2$lock) {
        if(search2$so.reserve(score)) {
            search2$so.nextInt(hash);
            search2$so.nextInt(hash0);
            search2$so.nextInt(score);
            search2$so.nextInt(score0);
            search2$so.nextInt(color);
        }
    }
    else {
        search2$lock = true;
        search2$so.init((170)+1, 2.9, 800);
        if(search2$so.reserve(score)) {
            search2$so.nextInt(hash);
            search2$so.nextInt(hash0);
            search2$so.nextInt(score);
            search2$so.nextInt(score0);
            search2$so.nextInt(color);
        }
        search2$loop();
        search2$lock = false;
    }
}

int main() {
    {
        const auto & $c1 = inputInt();
        const auto & $c2 = inputInt();
        const auto & $c3 = inputInt();
        tie(id, N, K) = tuple<int, int, int>($c1, $c2, $c3);
    }
    {
        S.resize(max((int)S.size(), (int)N));
        for(int $1=0, $e=(0<=N ? N : (int)S.size()+N); $1<$e; ++$1) S[$1] = inputWord();
    }
    {
        field.resize(max((int)field.size(), (int)10000));
        for(int $1=0, $e=(0<=10000 ? 10000 : (int)field.size()+10000); $1<$e; ++$1) field[$1] = S[$1 / 100][$1 % 100] - '1';
    }
    center = 4949;
    for(int $1=0, $e=(0<=10000 ? 10000 : (int)done.size()+10000); $1<$e; ++$1) done[$1] = 0;
    best = vector<int>();
    best3 = 0;
    Q[field[center]].push_back(center);
    search2(0, 0, 0, 0, field[center]);
    print(best.size() - 1);
    for(int $1=(0<=1 ? 1 : (int)best.size()+1), $e=(int)best.size(); $1<$e; ++$1) print(center / 100 + 1, center % 100 + 1, best[$1] + 1);
    return 0;
}
