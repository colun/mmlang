//#define NDEBUG
#ifdef mmlang_source_6_lines

A, B, H, M = inputInt(), inputInt(), inputInt(), inputInt()
M /= 60.0
H = (H + M) / 12.0
dx = cos(M * 2 * M_PI) * B - cos(H * 2 * M_PI) * A
dy = sin(M * 2 * M_PI) * B - sin(H * 2 * M_PI) * A
printf("%.20f\n", sqrt(dx*dx+dy*dy))

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
#include <cmath>

using namespace std;

// library code:

typedef long long int64;typedef __int128 int128;const char*inputCLineOrWord(int mode){static char buf[2097152];static int bufLen=sizeof(buf);static int bufPos=sizeof(buf);static bool canReadFlag=true;static bool crFlag=false;static bool enterFlag=false;
if(canReadFlag&&(enterFlag?bufLen<=bufPos:(int)sizeof(buf)<=bufPos+bufPos)){if(0<bufLen-bufPos){memmove(buf,buf+bufPos,bufLen-bufPos);bufLen-=bufPos;}else{bufLen=0;}char*result=fgets(buf+bufLen,sizeof(buf)-bufLen,stdin);canReadFlag=(result!=NULL);if(result!=NULL)
{int n=strlen(result);enterFlag=(n!=(int)sizeof(buf)-1-bufLen||(1<=bufLen+n&&buf[bufLen+n-1]=='\n'));bufLen+=n;}bufPos=0;}if(mode==0){int pos=bufPos;while(true){char c=buf[pos];if(c==32){buf[pos++]='\0';break;}else if(c==10){if(crFlag){crFlag=false;if(bufPos==pos)
{pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret=buf+bufPos;bufPos=pos;while(true){char c=buf[bufPos];if(c==32||c==10||c==13){++bufPos;}else{break;}}return ret;
}else if(mode==1){int pos=bufPos;while(true){char c=buf[pos];if(c==10){if(crFlag){crFlag=false;if(bufPos==pos){pos=++bufPos;continue;}}buf[pos++]='\0';break;}else if(c==13){crFlag=true;buf[pos++]='\0';break;}else if(c==0){break;}++pos;}const char*ret
=buf+bufPos;bufPos=pos;if(crFlag){while(true){char c=buf[bufPos];if(c==13){++bufPos;crFlag=false;break;}else{break;}}}return ret;}else if(mode==2){return bufLen<=bufPos?NULL:buf+bufPos;}assert(false);return NULL;}const char*inputCWord(){return inputCLineOrWord(0);
}int inputInt(){return atoi(inputCWord());}

// generated code (by mmlang ... https://github.com/colun/mmlang ) :

int A;
int B;
double H;
double M;
double dx;
double dy;

int main() {
    {
        const auto & $c1 = inputInt();
        const auto & $c2 = inputInt();
        const auto & $c3 = inputInt();
        const auto & $c4 = inputInt();
        tie(A, B, H, M) = tuple<int, int, int, int>($c1, $c2, $c3, $c4);
    }
    M /= 60.0;
    H = (H + M) / 12.0;
    dx = cos(M * 2 * M_PI) * B - cos(H * 2 * M_PI) * A;
    dy = sin(M * 2 * M_PI) * B - sin(H * 2 * M_PI) * A;
    printf("%.20f\n", sqrt(dx * dx + dy * dy));
    return 0;
}
