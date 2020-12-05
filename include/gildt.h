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

 void setgdtentry(gdt_entry* address,u32 base,u32 limit,u8 access,u8 flags);

