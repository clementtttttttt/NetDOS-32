#include<gildt.h>
#include<taskswitch.h>
#include<pman.h>
#include <string.h>
#include <kernel.h>
char c[16];
u32 read_eip();

#define dalign(unaligned) unaligned-((unaligned)&(0x400000-1))
#define ialign(unaligned) aligned+((-unaligned)&(0x400000-1))

void ill_op_c(u32 cs,u32 eip){
    printstring("eip: 0x");
    printstring(itoa(eip-0xfe588,c,16));
    while(1);
}

void c_gpf(u32 eip,u32 cs){
    printstring("\n****************GENERAL PROTECTION FAULT*******************\n");
    printstring("A general protection fault have happened. Don't worry, your \n");
    printstring("pc is completely fine. Here are the details:\n");
    printstring("eip: 0x");
    printstring(itoa(eip,c,16));
    while(1);
    
}
u32 get_cr3();
typedef struct taskinfo taskinfo;
TSS dummy_tss;
volatile taskinfo* c_task=0;
volatile taskinfo* next;
extern char stack_top;
void farjump(u32 eip,u32 cs);
u32 next_pid=1;
extern pagedir_t* pagedir;
extern pagedir_t* current_dir;

void init_task(){
    asm volatile("cli");
    c_task=next=(taskinfo*)kpmalloc(sizeof(taskinfo));
    c_task->id=next_pid++;
    c_task->task_tss.esp=c_task->task_tss.ebp=0;
    c_task->task_tss.eip=kernel_main;
    c_task->page_dir=current_dir;
    c_task->next=0;
    asm volatile("sti");
}

pagedir_t* clone_dir(pagedir_t* src){
    if(src==pagedir){
        return pagedir;
    }
    u32 phys;
    pagedir_t* clonedir=(pagedir_t*)kpmalloc_ap(sizeof(pagedir_t),&phys);
    memset(clonedir,0,sizeof(page[1024]));
    u32 off=(u32)clonedir->entrys-(u32)clonedir;
    clonedir->physaddr=phys+off;
    memcpy(clonedir,src,sizeof(pagedir_t));
    return clonedir;
}
int fork(u8* entry_addr){
    asm("cli");
    taskinfo *parent=(taskinfo*)c_task;
    taskinfo* new_task=(taskinfo*)kpmalloc(sizeof(taskinfo));
    new_task->id=next_pid++;
    new_task->task_tss.esp=new_task->task_tss.ebp=0;
    new_task->task_tss.eip=entry_addr;
    new_task->page_dir=clone_dir(current_dir);
    new_task->next=0;

    taskinfo *tmp=(taskinfo*)next;
    while(tmp->next)
        tmp=tmp->next;

    tmp->next=new_task;
    u32 eip=read_eip();
    if(c_task==parent){
        u32 esp,ebp;
        asm volatile("mov %%esp,%0;mov %%ebp,%1":"=r"(esp),"=r"(ebp));
        new_task->task_tss.esp=esp;
        new_task->task_tss.ebp=ebp;
        new_task->task_tss.eip=eip;
        asm volatile("sti");
        return new_task->id;
    }
    else {
        return 0;
    }
}
void task_a(){

    while(1);
            
}
        u32 internal_i=0;

extern gdt_entry gdt[5];
u32 ltr_();
void kernel_task();
#include <mutex.h>
#include <string.h>
volatile bool taskswitch_lock;
extern char tss;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool init=false;
void taskswitch(){

            u32 resp,rebp,reip;
        reip=read_eip();

    if(!c_task) return;
        asm volatile("mov %%esp, %0" : "=r"(resp));
        asm volatile("mov %%ebp, %0" : "=r"(rebp));        
        if(reip==0x12345){
            return;
        }
        if(init==false){
            c_task->task_tss.eip=kernel_main;
            init=true;
            
        }
        else c_task->task_tss.eip=reip;
        
        c_task->task_tss.esp=resp;
        c_task->task_tss.ebp=rebp;
        c_task=c_task->next;
        if(c_task==0)c_task=next;
        
        reip=c_task->task_tss.eip;
        resp=c_task->task_tss.esp;
        rebp=c_task->task_tss.ebp;
        current_dir=c_task->page_dir;
                asm volatile("cli");

        asm volatile("         \
        mov %0, %%ecx;       \
        mov %1, %%esp;       \
        mov %2, %%ebp;       \
        mov %3, %%cr3;       \
        mov $0x12345, %%eax; \
        sti;\
        jmp *%%ecx           "
                : : "r"(reip), "r"(resp), "r"(rebp), "r"(c_task->page_dir->entrys));
    
    
}

void timer(){
    taskswitch();
    
    
    
}

#pragma GCC diagnostic pop
