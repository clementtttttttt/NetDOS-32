#include "kernel.h"
#include "font.h"
#include "taskswitch.h"
extern u32 read_eip();
void kernel_main(){
     printstring("Hello, I'm kernel_main() (AKA kernel task), hope you'll be satisfied of my service!\n");
    while(1)    ;
}

void start_kernel_c(){
    printstring("Initializing multitasking and kernel task...\n");
    init_task(); //jumpstarts the kernel_main function, aka the actual kernel.
    while(1);
}   
