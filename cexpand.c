extern void hang();
#include <multiboot.h>
multiboot_info_t* public_mbd;
#include "font.h"
#include "kernel.h"
unsigned int public_magic;
char stringbuffer[100]="ABCD";
unsigned char rootdisktype;
unsigned char drivenum;
char getmultiboot(multiboot_info_t* mbd,unsigned int magic){
    public_magic=magic;
    terminal_initialize();
    if(magic!=MULTIBOOT_BOOTLOADER_MAGIC){
        printstring("Invalid bootloader magic number(Should be 0x2BADB002), dumped in EAX. HALT.\n");
        asm("movl public_magic,%EAX");
        hang();
    }
    switch(mbd->boot_device>>28){
        case 0xe:
            printstring("Root drive is cdrom\n");
            rootdisktype=0x3;
            break;
        case 0x8:
            printstring("Root drive is hdd\n");
            rootdisktype=0x2;
            break;
    
        
    }
        public_mbd=mbd;

    drivenum=mbd->boot_device>>24&0b1111;
    return drivenum;
    
}
