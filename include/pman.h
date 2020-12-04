#ifndef pman_h_i
#define pman_h_i


#define dalign(unaligned) unaligned-((unaligned)&(0x400000-1))
#define ialign(unaligned) aligned+((-unaligned)&(0x400000-1))
typedef unsigned int u32;
typedef unsigned short u16;
extern void* endadr;

#define uintn_t unsigned long


enum{
    NPROT=1,RPROT,SPROT=0b100,ERRNES=0x401,ERRNEP
    
};
/*-----------------------------paging---------------------------------------*/


typedef struct page_flags{
        unsigned char flags_p:1;
        unsigned char flags_r:1;
        unsigned char flags_u:1;
        unsigned char flags_w:1;
        unsigned char flags_d:1;
        unsigned char flags_a:1;
        unsigned char flags_s:1;
        unsigned char flags_g:1;
}page_flags;
struct struct_page{
    union{
        page_flags flags_bitfield;
        unsigned char flags;
    };
    unsigned int addr : 24;
    
        
}__attribute__((aligned(4)));
void initpage();
typedef struct struct_page struct_page;
typedef union page{
    struct_page struct_p;
    uintn_t raw;
    
}page;
typedef struct pagedir_t{
        
    page entrys[1024];
    unsigned int phys[1024];
    unsigned int physaddr;
    
} pagedir_t;

uintn_t kpmalloc(uintn_t sz);

#define GET_VARIABLE_NAME(Variable) (#Variable)

#endif
