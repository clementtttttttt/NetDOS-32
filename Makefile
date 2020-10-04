OBJ=$(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst blibc/%.c,blibc/%.o,$(wildcard blibc/*.c))
CC=i686-elf-gcc
CFLAGS=-Wextra -Wall -O2 -s -Wno-int-to-pointer-cast -Wno-sign-compare -Wno-address 
NASM=nasm
NFLAGS=-felf32 
LD=i686-elf-gcc
all: os.iso

os.iso:kernel.elf
	-@mkdir -p isodir/boot/grub
	@cp kernel.elf isodir/boot/kernel.elf
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@sudo grub-mkrescue -o os.iso isodir
	
	
boot.o:boot.asm
	@echo "[NASM($(NASM))] $<"
	@$(NASM) -o $@ $<  $(NFLAGS)
%.o:%.c Makefile include/font.h 
	@echo "[CC($(CC))] $<" 
	@$(CC) $(CFLAGS) -ffreestanding -std=gnu99 -c -o $@ $< -I /usr/include/multiboot/ -I blibc/include -nostdlib -I include  
kernel.elf:$(OBJ) boot.o 
	@echo "[LD($(LD))] $(OBJ) boot.o"
	@$(LD)  -Tlinker.ld -o $@ -ffreestanding -O2 -nostdlib  boot.o  $(OBJ) -lgcc   $(CFLAGS) 
clean:
	-rm *.o *.elf blibc/*.o
test4g: all
	qemu-system-x86_64 -cdrom os.iso -m 4G
test: all
	qemu-system-i386 -cdrom os.iso
