#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <algorithm>

using namespace std;

#define w while
#define gl getline
#define c cin
#define s string
#define f fill
#define o cout
#define r return

int main() {
    s l;int n=12,g=0,co[n];f(co,co+n,0);w(gl(c,l))for(int i=0;i<n;++i)co[i]+=l[i]=='1'?1:-1;for(int i=0;i<n;++i)if(co[i]>0)g+=pow(2,n-i-1);o<<g*(g^0b111111111111);
    r 0;
}

