#define align(unaligned) unaligned+((-unaligned)&(0x400000-1))
extern multiboot_info_t* public_mbd;
extern void printstring(char*);
extern char* itoa(unsigned int, char*, int);
#define uintn_t unsigned long
extern int tmb;
enum{
    NPROT=1,RPROT,SPROT=0b100,ERRNES=0x401,ERRNEP
    
};
/*-----------------------------paging---------------------------------------*/
uint32_t pagedir[1024]__attribute__((aligned(4096)));
uint32_t fpt[1024]__attribute__((aligned(4096)));
uint32_t ramsize;
unsigned int memtest(unsigned int start,unsigned int end);
void loadpagedir(uint32_t* pagedir);
void enablepage();
extern char* a;
uint8_t* pman_bitfield=(uint8_t*)0x400000;
extern const char* ok;
int pmap(uint32_t size,uint16_t flags,uintn_t adr){
    flags&=0b111000000111;
    uint32_t pcount=(align(size))/0x400000;
    for(uintn_t i=0;(int)(pman_bitfield+i)<=((ramsize/4194304/8)+0x400000);i++){
        for(char bit=0;bit<=7;bit++){
            if(!((*pman_bitfield+i)&(1<<bit))){
                
                break;
            }
        }
    }
    
}
void initpage(){
    printstring("Initializing paging...");
    ramsize=memtest(0x800000,0xbfffffff);
    printstring("RAM ");
    printstring(itoa(ramsize,a,10));
    printstring("");
    for(int i=0;i<1024;i++) pagedir[i]=2;
    printstring(" B OK\n");
    pagedir[0]=(uint32_t)0b10000011;
    pagedir[1]=((uint32_t)0x400000)|0b10000011;
    pagedir[2]=((uint32_t)0x800000)|0b10000011;
    if(!tmb)
    pagedir[(uint32_t)public_mbd->framebuffer_addr>>22]=((uint32_t)public_mbd->framebuffer_addr)|0b10000111;
    loadpagedir(pagedir);
    enablepage();
    return;
    
}
