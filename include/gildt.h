typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct gdt_entry
{
	unsigned int limit_low:16;
	unsigned int base_low : 24;
     //attribute byte split into bitfields
	unsigned char access;
     //and now into granularity
	unsigned int limit_high :4;
	unsigned int flags:4;
	unsigned char base_high;
} __attribute__((packed)); //or __attribute__((packed))
 typedef struct gdt_entry gdt_entry;

typedef struct idt_entry{
    u16 offset_l,selector;
    u8 set_to_zero,type_attr;
    u16 offset_h;
} idt_entry;
 
void setgdtentry(gdt_entry* address,u32 base,u32 limit,u8 access,u8 flags);

void setidtentry(idt_entry* address,u16 offset_l,u16 selector,u8 type_attr,u16 offset_h);
