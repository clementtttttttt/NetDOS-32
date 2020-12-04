#include <stdint.h>
#include "io.h"
                char a[100]={0};
extern char* itoa(unsigned int, char*, int);
extern void printstring();
uint16_t readconfigword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t cfw = 0;
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl (0xcf8,address);
    cfw=(uint16_t) ((inl(0xcfc)>>((offset&2)*8))&0xffff);
    return cfw;
}
void scandev(){
    for(uint16_t bus=0;bus<256;bus++){
            for(uint8_t device=0;device<32;device++){
                for(int func=0;func<8;func++){
                    if(readconfigword(bus,device,func,0)==0xffff){
                        goto skip;
                    }
                    printstring("PCI device detected in bus ");
                    itoa(bus,a,10);
                    printstring(a);
                    printstring(",is device no.");
                    itoa(device,a,10);
                    printstring(a);
                    printstring(" function ");
                    printstring(itoa(func,a,10));
                    printstring(",id ");
                    printstring(itoa(readconfigword(bus,device,func,0),a,16));
                    printstring(":");
                    printstring(itoa((uint32_t)readconfigword(bus,device,func,2),a,16));
                    printstring(" class code ");
                    printstring(itoa(((uint32_t)readconfigword(bus,device,func,10)&0xff00)>>8,a,16));
                    printstring(" subclass code ");
                    printstring(itoa(((uint32_t)readconfigword(bus,device,func,10)&0xff),a,16));
                    printstring("\n");
                    skip:;
                }
            }
    }
}
