#include <multiboot.h>
#include "kernel.h"
#include "font.h"
#include "taskswitch.h"
#include "heap.h"
#include <stdlib.h>
#include "syscall.h"
extern u32 read_eip();
KHEAPSS heap;

long rootuuid=0;


extern multiboot_info_t* public_mbd;
u32 eaxv=0;
char tt[100];
long strtrunc(char* str,char ter){
    long i=0;
    char* strbac=str;
    while(str[i]!=ter)++i;
    str[i]=0;
    return i;
}
void parse_kernel_cmdline(){
    char* cmdline=public_mbd->cmdline;
    for(int i=0;cmdline[i]!=0;i++){
        if(cmdline[i]=='r'&&cmdline[i+1]=='o'&&cmdline[i+2]=='o'&&cmdline[i+3]=='t'){
            i+=5;
            long off=strtrunc(cmdline+i,32);
            rootuuid=atoi(cmdline+i);
            i+=off+1;
            
        }

    }
    if(rootuuid==0){
            asm("cli");
            printstring_v("*****FATAL ERROR: ROOT DIRECTIVE NOT SPECIFIED*****");
            asm("hlt");
    }
}
extern void hang();
void user_task(){
    asm("int $0x80");
    while(1){
    }
}
void kernel_main(){
    parse_kernel_cmdline();
    printstring("\nHello, I'm kernel_main() (AKA kernel task), hope you'll be satisfied of my service!");
    ntask(user_task,false);

    printstring("\ngetpid() returns ");
    printstring(itoa(getpid(),tt,10));
    printstring("\nnothing left to do. resting.");
    asm("sti");
    while(1);
}

void start_kernel_c(){
    asm("cli");
    init_syscall();
    printstring("\nInitializing kernel heap...");
    init_heap(&heap,4096);
    heapaddblock(&heap,0xE0000000,0x20000);
    printstring("OK\n");
    printstring("Initializing Ring 3 TSS...");
    init_user();
    printstring("OK\n");
    printstring("Initializing multitasking and kernel task...");
    init_task(); //starts the kernel_main function, aka the actual kernel.
    while(1);
}   
