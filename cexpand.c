extern void hang();
#include <multiboot.h>
multiboot_info_t* public_mbd;
#include "font.h"
#include <stdio.h>
#include "kernel.h"
char stringbuffer[100]="ABCD";
void getmultiboot(multiboot_info_t* mbd,unsigned int magic){
    if(magic!=MULTIBOOT_BOOTLOADER_MAGIC){
        printstring("Invalid bootloader magic number. HALT.");
        hang();
    }
    public_mbd=mbd;
    
}
