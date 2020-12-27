#include<stdlib.h>



long atoi(const char* s){

    int i=0,n=0;

    for(i;s[i]>='0' && s[i]<='9';i++)
        n=10*n + (s[i]-'0');
    return n;
}
