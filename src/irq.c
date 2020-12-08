void c_irq7(){
    
}
unsigned char scancode_2;
void c_irq1(unsigned char scancode){
    if(scancode==0xe0){
            asm("mov $0,%eax;inb $0x60,%al");
            
        
    }
    
}
