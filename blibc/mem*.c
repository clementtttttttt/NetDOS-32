typedef unsigned int u32;
typedef unsigned long long u64;
#include"mutex.h"
bool memcpy_lock;
void* 
memcpy(void* dest,void* src, u64 count){
   char *csrc = (char *)src; 
   char *cdest = (char *)dest; 
  
   // Copy contents of src[] to dest[] 
   for (u64 i=0; i<count; i++) 
       cdest[i] = csrc[i]; 
   return dest;
}
void*
memset(unsigned char* dest,u32 val,u64 count){
    for(u64 i=0;i<=count;i++){
        *(dest+i)=(unsigned char)val;
    }
    return dest;
}
