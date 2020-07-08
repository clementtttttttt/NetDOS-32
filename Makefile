OBJ=$(patsubst %.c,%.o,$(wildcard *.c))
CC=clang
CFLAGS=-Wextra -Wall -O2
NASM=nasm
NFLAGS=-felf32
LD=i686-elf-ld
all: os.iso 

os.iso:kernel.elf
	-@mkdir -p isodir/boot/grub
	@cp kernel.elf isodir/boot/kernel.elf
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@sudo grub-mkrescue -o os.iso isodir
	
boot.o:boot.asm
	@echo "[NASM] $<"
	@$(NASM) -o $@ $<  $(NFLAGS)
%.o:%.c Makefile 
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -ffreestanding -std=gnu99 --target=i686-pc-elf -c -o $@ $< -I /usr/include/multiboot/
kernel.elf:$(OBJ) boot.o
	@echo "[LD] $(OBJ) boot.o"
	@$(LD) -Tlinker.ld -nostdlib -lgcc boot.o $(OBJ) -L./i686-elf/lib/gcc/i686-elf/10.1.0 -L./i686-elf-glibgc/lib -o $@
clean:
	-rm *.o *.elf 
test: all
	qemu-system-x86_64 -cdrom os.iso
