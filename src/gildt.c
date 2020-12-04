#include <gildt.h>
void setgdtentry(gdt_entry* address,u32 base,u32 limit,u8 access,u8 flags){
    address->base_low=base&0xffffff;
    address->limit_low=limit&0xffff;
    address->access=access;
    address->limit_high=(limit&0xf0000)>>16;
    address->base_high=(base&0xff000000)>>24;
    asm("lgdt gdtinfo");
}
