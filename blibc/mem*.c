typedef unsigned int u32;
typedef unsigned long long u64;
#include"mutex.h"
bool memcpy_lock;
void* 
memcpy(char* dest,char* src, u64 count){
    for(u64 i=0;i<count;i++){
            dest[i]=src[i];
    }
    return dest;
}
void*
memset(unsigned char* dest,u32 val,u64 count){
    for(u64 i=0;i<=count;i++){
        *(dest+i)=(unsigned char)val;
    }
    return dest;
}
