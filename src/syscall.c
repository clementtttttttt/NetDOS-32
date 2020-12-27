#include <syscall.h> 
#include <font.h>
#include <taskswitch.h>
#include <gildt.h>

static void sc_handler(registers_t* regs);

static void* syscalls[10]={
    &printstring
};

void syscall_handler(){
   // printstring("test");
}

extern void int0x80;
unsigned int num_syscalls=(sizeof(syscalls)/sizeof(void*));
extern idt_entry idt[0x84] ;
void init_syscall(){
    setidtentry(&idt[0x80],(u16)&int0x80,0x8,0b11101110,((u32)&int0x80>>16));
}

DEFN_SC1(printstring,0,const char*)

