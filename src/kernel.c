#include "kernel.h"
#include "font.h"
#include "taskswitch.h"
#include "heap.h"
extern u32 read_eip();
KHEAPSS heap;
u32 eaxv=0;
char tt[100];

void kernel_main(){
    
    printstring("\nHello, I'm kernel_main() (AKA kernel task), hope you'll be satisfied of my service!");
    char tt[100];
    printstring("\nInitializing Ring 3 TSS...");
    init_user();
/*    printstring("\ngetpid() returns ");
    printstring(itoa(getpid(),tt,10));
    printstring("\nnothing left to do. resting.");*/
    while(1);
}

void start_kernel_c(){
    asm("cli");

    printstring("OK\nInitializing kernel heap...");
    init_heap(&heap,4096);
    heapaddblock(&heap,0xE0000000,0x20000);
    printstring("OK\n");
    printstring("Initializing multitasking and kernel task...");
    init_task(); //starts the kernel_main function, aka the actual kernel.
    while(1);
}   
