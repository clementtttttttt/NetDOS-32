#include<gildt.h>
#include<taskswitch.h>
#include<pman.h>
#include <string.h>
#include <kernel.h>
#include <heap.h>
char c[16];
u32 read_eip();
extern char stack_top;

#define dalign(unaligned) unaligned-((unaligned)&(0x400000-1))
#define ialign(unaligned) aligned+((-unaligned)&(0x400000-1))
TSS user_tss;
extern gdt_entry gdt[5];
void init_user(){
    setgdtentry(&gdt[5],&user_tss,sizeof(TSS),0x89,0x40);
    memset(&user_tss,0,sizeof(TSS));
    user_tss.ss0=0x10;
    user_tss.esp0=&stack_top;
    user_tss.cs=0x0b;
    user_tss.ss=user_tss.ds=user_tss.es=user_tss.fs=user_tss.gs=0x13;
    asm volatile("mov $0x2b,%ax;ltr %ax");
}
void ring3(){
      asm volatile("  \
     cli; \
     mov $0x23, %ax; \
     mov %ax, %ds; \
     mov %ax, %es; \
     mov %ax, %fs; \
     mov %ax, %gs; \
                   \
     mov %esp, %eax; \
     pushl $0x23; \
     pushl %eax; \
     pushf; \
     pushl $0x1B; \
     push $1f; \
     iret; \
   1: \
     ");
}

void ill_op_c(u32 ss,u32 esp,u32 eflags,u32 cs,u32 eip){
    printstring("\nInvalid opcode, eip: 0x");
    printstring(itoa(esp,c,16));
    asm("cli;hlt");
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
volatile taskinfo* c_task=0;
volatile taskinfo* next;
void farjump(u32 eip,u32 cs);
u32 next_pid=1;
extern pagedir_t* pagedir;
extern pagedir_t* current_dir;
_Bool task_init=0;

void init_task(){
    asm volatile("cli");
    task_init=1;

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
    memset(clonedir,0,sizeof(pagedir_t));
    u32 off=(u32)clonedir->entrys-(u32)clonedir;
    clonedir->physaddr=phys+off;
    memcpy(clonedir,src,sizeof(pagedir_t));
    return clonedir;
}
//TODO:supervisor bit
int ntask(u8* thread,_Bool iss){
    asm("cli");
    taskinfo *parent=(taskinfo*)c_task;
    pagedir_t* dir=clone_dir(current_dir);
    taskinfo* new=(taskinfo*)kpmalloc(sizeof(taskinfo));
    new->id=next_pid++;
    new->task_tss.esp=new->task_tss.ebp=0;
    new->task_tss.eip=0;

    new->page_dir=dir;
    new->next=0;
    taskinfo* tmp=(taskinfo*)next;

    while(tmp->next)
        tmp=tmp->next;

    tmp->next=new;
    if(c_task==parent){
      u32 esp; asm volatile("mov %%esp, %0" : "=r"(esp));
       u32 ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));    
        new->task_tss.esp=khmalloc(4096);
 
        new->task_tss.ebp=ebp;
        new->task_tss.eip=thread;
        asm volatile("sti");
        return new->id;
    }
    else{ return 0;

        
    }
}
        u32 internal_i=0;

void kernel_task();
#include <mutex.h>
#include <string.h>
volatile bool taskswitch_lock;
extern char tss;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool init=false;
#pragma GCC push_options
#pragma GCC optimize "-O3"
registers_t* gregs;

//TODO:signal handling,ring switch,process killing,return handler and such
void taskswitch(){
           u32 resp,rebp,reip;
    if(!c_task) return;
        asm volatile("mov %%esp, %0" : "=r"(resp));
        asm volatile("mov %%ebp, %0" : "=r"(rebp));        
            reip=read_eip();

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
                char tt[1];
        itoa(1,tt,16);

        asm volatile("      cli;\
        mov %1, %%esp;       \
        mov %2, %%ebp;       \
        mov %3, %%cr3;       \
        mov $0x12345, %%eax; \
        sti;\
        jmp *%0           "
                : : "r"(reip), "r"(resp), "r"(rebp), "r"(current_dir->entrys));
    
    
}
#pragma GCC pop_options
void timer(registers_t regs){
    taskswitch();
    
    
    
}
u32 getpid(){
    return c_task->id;
}

#pragma GCC diagnostic pop
