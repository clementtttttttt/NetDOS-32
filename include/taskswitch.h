#include <pman.h>

typedef unsigned char u8;
#define bool _Bool

typedef struct registers
{
   u32 ds;                  // Data segment selector
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   u32 int_no, err_code;    // Interrupt number and error code (if applicable)
   u32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t; 
struct TSS{
    unsigned int backlink,esp0,ss0,esp1,ss1,esp2,ss2,cr3,eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi,es,cs,ss,ds,fs,gs,ldtr,tiomapb;
    
    
}__attribute__((packed));

typedef struct TSS TSS;
enum states{
    n=0,NORM=1,SEGV,MATHE,KILL,TERM,INT,CUS1,CUS2,CUS3,CUS4,CUS5,CUS6,end
};
//TODO: info_bitfield
/*info_bitfield 

|0|1|2|3|4|5|6|7|
0:supervisor bit
1:
*/
struct taskinfo{
    registers_t task_tss;
    u32 state;
    u32 id;
    void(*state_handler[end-1])(void);
    u8 info_bitfield;
    struct taskinfo* next;
    pagedir_t* page_dir;
};
void init_task();
int ntask(u8* thread,_Bool issupervisor);
void init_user();
