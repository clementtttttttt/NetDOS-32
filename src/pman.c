#include<pman.h> 
#include<multiboot.h>
#include<stdint.h>
#include<taskswitch.h>
extern int tmb;
uintn_t place_addr=&endadr;
extern taskinfo* c_task;
extern multiboot_info_t* public_mbd;
extern void printstring(char*);
extern char* itoa(unsigned int, char*, int);
char temp[33];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void c_pfh(u32 errcode,u32 eip,u16 cs){
    asm("cli");
    printstring_v("\n*************Page Fault**************\nA page fault have occured. Stay calm, it won't kill your computer. If you are not a developer, ignore this message. Or else, read the details below. They helps.\nError code: 0b");
    printstring_v(itoa(errcode,temp,2));
    printstring_v(" cs: 0x");
    printstring_v(itoa(cs,temp,16));
    printstring_v(" eip: 0x");
    printstring_v(itoa(eip,temp,16));

    if (c_task->id==1){
        printstring_v("\nThis is a kernel panic. Will not attempt to recover. The devs give you the most sincere apologies if you're in a hurry.\n********Reset your computer.*********");
        while(1);
    }
    printstring_v("\nKilling trigger process...\n");
    c_task->state=SEGV;
    asm("movw $0x10,%ax;mov %ax,%ds;mov %ax,%es;mov %ax,%fs;mov %ax,%gs;mov %ax,%ss;");
    asm("popl %eax;popl %eax;popl %eax;popl %eax;popl %eax;");
    asm("jmp taskswitch");
    while(1);
   
}
#pragma GCC diagnostic pop
/*kpmalloc functions*/
uintn_t kpmalloc(uintn_t sz){
    place_addr+=sz;
    return place_addr-sz;
}
uintn_t kpmalloc_a(uintn_t sz){
    if(place_addr&0x00000fff){
        place_addr&=0xfffff000;
        place_addr+=0x1000;
    }
    uintn_t tmp=place_addr;
    place_addr+=sz;
    return tmp;
    
}
uintn_t kpmalloc_p(uintn_t sz,uintn_t *phys){
    if(phys){
        *phys=place_addr;
    }
    uintn_t tmp=place_addr;
    place_addr+=sz;
    return tmp;
    
}
uintn_t kpmalloc_ap(uintn_t sz, uintn_t *phys)
{
  if ((place_addr & 0x00000fff)) // If the address is not already page-aligned
  {
    // Align it.
    place_addr &= 0xFFFFF000;
    place_addr += 0x1000;
  }
  if (phys)
  {
    *phys = place_addr;
  }
  uintn_t tmp = place_addr;
  place_addr += sz;
  return tmp;
} 
/*end kpmalloc functions*/

pagedir_t* pagedir;
pagedir_t* current_dir;
uint32_t fpt[1024]__attribute__((aligned(4096)));
uint32_t ramsize;
unsigned int memtest(unsigned int start,unsigned  int end);
void loadpagedir(uint32_t* pagedir);
void enablepage();
extern const char* ok;
int pmap(uintn_t physical_addr,uintn_t v_addr,uintn_t page_count){
    asm("cli");
    for(int i=0;page_count<=i;++i){
        
    }
    asm("sti");
    
}
char a[100]={0};
void initpage(){
    printstring("Initializing paging...");
    ramsize=memtest(0x800000,0xbfffffff);
    printstring("RAM ");
    printstring(itoa(ramsize,a,10));
    printstring("");
    printstring(" B OK");
    pagedir=(pagedir_t*)kpmalloc_a(sizeof(pagedir_t));
    memset(pagedir,0,sizeof(pagedir_t));
    current_dir=pagedir;
    int i=0;
    while(i*0x400000<place_addr){
        pagedir->entrys[i].struct_p.flags=(uint32_t)0b10000101;
        pagedir->entrys[i].struct_p.addr=i*0x400000;
        ++i;
    }
    i=0;
    while(i<128){
          pagedir->entrys[i+896].struct_p.flags=(uint32_t)0b10000101;
        pagedir->entrys[i+896].struct_p.addr=i*0x400000;
        ++i;
    }
    if(!tmb){
        pagedir->entrys[(uint32_t)public_mbd->framebuffer_addr>>22].raw=((uint32_t)public_mbd->framebuffer_addr)|0b10000101;
        pagedir->entrys[((uint32_t)public_mbd->framebuffer_addr>>22)-1].raw=((uint32_t)public_mbd->framebuffer_addr-0x400000)|0b10000101;
        pagedir->entrys[((uint32_t)public_mbd->framebuffer_addr>>22)+1].raw=((uint32_t)public_mbd->framebuffer_addr+0x400000)|0b10000101;
        
        
    }
    loadpagedir(pagedir->entrys);
    enablepage();
    return;
    
}
