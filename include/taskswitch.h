#include <pman.h>

typedef unsigned char u8;
#define bool _Bool
struct TSS{
    unsigned int backlink,esp0,ss0,esp1,ss1,esp2,ss2,cr3,eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi,es,cs,ss,ds,fs,gs,ldtr,iopb;
    
    
}__attribute__(());

typedef struct TSS TSS;
enum states{
    NP=0,NORM=1,SEGV,MATHE,KILL,TERM,INT,CUS1,CUS2,CUS3,CUS4,CUS5,CUS6,end
};
struct taskinfo{
    TSS task_tss;
    u32 state;
    u32 id;
    void(*state_handler[end-2])(void);
    u8 info_bitfield;
    u32 next;
    pagedir_t* page_dir;
};
void init_task();
void create_task(u8* entry_addr,bool enable_fork);
