; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1           ; provide memory map
FLAGS    equ  0b00000000000000000000000000000111 ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot
 	extern getmultiboot
extern printstring
extern _kernel_start
extern _kernel_end
; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
global stack_top
section .bss
	align 4096

stack_bottom:
    resb 16384*4 ; 16 KiB
stack_top:
    resb 0

section .multiboot
global tmb
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
    dq 0
    dq 0
    dd 0
tmb dd 0
    dd 1920
    dd 1080
    dd 0
kdata:
  .ctrlbit db 0
    .shiftbit db 0
    .temp db 0
    .lookup_table_shift dw  0
        db "!@#$%^&*()_+",8h,9h,"QWERTYUIOP{}",10,0,"ASDFGHJKL:",22h,'~',0x7a,0x7c,"ZXCVBNM",'<','>','?',0,0,0,20h,0x2a
        times (0xe5-0x3b) db 0
    .lookup_table dw 0
        db "1234567890-=",8h,9h,"qwertyuiop[]",10,0x9e,"asdfghjkl;",27h,'`',0x7a,"\zxcvbnm,./",0,0,0,20h,'C'
        times (0xe5-0x3b) db 0
; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
; Declare _start as a function symbol with the given symbol size.
section .text
global _start:function (end - _start)
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled. The processor
	; state is as defined in the multiboot standard. The kernel has full
	; control of the CPU. The kernel can only make use of hardware features
	; and any code it provides as part of itself. There's no printf
	; function, unless the kernel provides its own <stdio.h> header and a
	; printf implementation. There are no security restrictions, no
	; safeguards, no debugging mechanisms, only what the kernel provides
	; itself. It has absolute and complete power over the
    
; 

 jmp gdt_end
gdtinfo:
   dw gdt_end - gdt - 1   ;last byte in table
   dd gdt                 ;start of table
 global gdt
gdt         dd 0,0        ; entry 0 is always unused
flatcode    db 0xff, 0xff, 0, 0, 0, 10011010b, 11001111b, 0
flatdata    db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
usercode    db 0xff,0xff,0,0,0,0xfa,0xcf,0
userdata    db 0xff,0xff,0,0,0,0xf2,0xcf,0;
tss         db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0

v86:
jmp dword [v86_end]
nop
v86_end:
dd 0
gdt_end:
	; To set up a stack, we set the esp register to point to the top of our
	; stack (as it grows downwards on x86 systems). This is necessarily done
	; in assembly as languages such as C cannot function without a stack.
	mov esp, stack_top
	mov ebp,stack_top
    global gdtinfo	; This is a good place to initialize crucial processor state before the
	lgdt[gdtinfo]

    jmp 0x8:load
load:
    cli
   
  mov dx,0x10
    mov ds,dx
    mov es,dx
    mov fs,dx
    mov gs,dx
    mov ss,dx
mov cl,al
disable_cursor:
    mov dx,0x3d4
    mov al,0xa
    out dx,al
    inc dx
    mov al,0x20
    out dx,al
    mov al,cl
    pushad

change:
    cmp byte [tmb],1
    jne cend
    mov esi,regs_90x60
    call write_regs
    
cend:
    popad

    ;high-level kernel is entered. It's best to minimize the early
	; environment where crucial features are offline. Note that the
	; processor is not fully initialized yet: Features such as floating
	; point instructions and instruction set extensions are not initialized
	; yet. The GDT should be loaded here. Paging should be enabled here.s
	; C++ features such as global constructors and exceptions will require
	; runtime support to work as well.
 
	; Enter the high-level kernel. The ABI requires the stack is 16-byte
	; aligned at the time of the call instruction (which afterwards pushes
	; the return pointer of size 4 bytes). The stack was originally 16-byte
	; aligned above and we've since pushed a multiple of 16 bytes to the
	; stack since (pushed 0 bytes so far) and the alignment is thus
	; preserved and the call is well defined.
        ; note, that if you are building on Windows, C functions may have "_" prefix in assembly: _kernel_main
    push eax
    push ebx
    call getmultiboot  

    pop edx
    pop edx
version:
    push versionstring
    call printstring
    pop edx
    mov [rootdrvnum],eax
	cli
initpic:
    push pic2 
    call printstring
    pop edx
    mov al,0x11
    out 0x20,al
    call iowait
    out 0xa0,al
    call iowait
    mov al,0x20
    out 0x21,al
    call iowait
    mov al,0x28
    out 0xa1,al
    call iowait
    mov al,4
    out 0x21,al
    call iowait

    mov al,2
    out 0xa1,al
    call iowait
    mov al,1
    out 0x21,al
    call iowait
    out 0xa1,al
    call iowait
    mov eax,irq0
    mov [idt.pitaddr],ax
    mov eax,irq0
    shr eax,16
    mov [idt.pitaddrh],ax
    mov eax,irq1
    mov [idt.keyaddr],ax
    mov eax,irq1
    shr eax,16
    mov [idt.keyaddrh],ax
    mov eax,pagefaulthandler
    mov [idt.pfaddr],ax
    shr eax,16
    mov [idt.pfaddrh],ax
extern c_gpf
extern irq7handler
extern ill_op_c
    mov eax,ill_op_c
    mov [idt.illopaddr],ax
    shr eax,16
    mov [idt.illopaddrh],ax
    mov eax,c_gpf
    mov [idt.gpfaddr],ax
    shr eax,16
    mov [idt.gpfaddrh],ax
    mov eax,irq7handler
    mov [idt.irq7addr],ax
    shr eax,16
    mov [idt.irq7addrh],ax
    lidt[idtinfo]
    push ok
    call printstring
    pop edx
    mov al,0
    out 0x21,al
paging:
    extern initpage
   call initpage
 

  global hang             
setup_pit:
    mov al,0x36
    out 0x43,al
    mov ax,23863*2;
    out 0x40,al
    mov al,ah
    out 0x40,al
setup_multitasking:
    sti
    

kernel_task:
    mov al,0b11111110
    out 0x21,al
extern start_kernel_c
    jmp start_kernel_c
hang:
    jmp $
end:
global read_eip
read_eip:
    mov eax,[esp]
    ret
  
VGA_MISC_WRITE		EQU	3C2h
VGA_SEQ_INDEX		EQU	3C4h
VGA_SEQ_DATA		EQU	3C5h
; VGA_DAC-READ_INDEX
; VGA_DAC_WRITE_INDEX
; VGA_DAC_DATA
; VGA_MISC_READ
VGA_CRTC_INDEX		EQU	3D4h
VGA_CRTC_DATA		EQU	3D5h
VGA_INSTAT_READ		EQU	3DAh

NUM_SEQ_REGS		EQU	5
NUM_CRTC_REGS		EQU	25

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; name:		write_regs
; action:	writes register dump to VGA registers
; inputs:	SI->register dump
; outputs:	(nothing)
; modifies:	(nothing)
; minimum CPU:	8088
; notes:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global get_cr3
get_cr3:
    mov eax,cr3
    ret
write_regs:
	push si
	push dx
	push cx
	push ax
		cld

; write MISC register
		mov dx,VGA_MISC_WRITE
		lodsb
		out dx,al

; write SEQuencer registers
		mov cx,NUM_SEQ_REGS
		mov ah,0
write_seq:
		mov dx,VGA_SEQ_INDEX
		mov al,ah
		out dx,al

		mov dx,VGA_SEQ_DATA
		lodsb
		out dx,al

		inc ah
		loop write_seq

; write CRTC registers
; Unlock CRTC registers: enable writes to CRTC regs 0-7
		mov dx,VGA_CRTC_INDEX
		and al,7Fh
		out dx,al

; Unlock CRTC registers: enable access to vertical retrace regs
		mov dx,VGA_CRTC_INDEX
		mov al,3
		out dx,al

		mov dx,VGA_CRTC_DATA
		in al,dx
		or al,80h
		out dx,al

; make sure CRTC registers remain unlocked
		mov al,[si + 17]
		and al,7Fh
		mov [si + 17],al

		mov al,[si + 3]
		or al,80h
		mov [si + 3],al

; now, finally, write them
		mov cx,NUM_CRTC_REGS
		mov ah,0
write_crtc:
		mov dx,VGA_CRTC_INDEX
		mov al,ah
		out dx,al

		mov dx,VGA_CRTC_DATA
		lodsb
		out dx,al

		inc ah
		loop write_crtc
	pop ax
		mov al,17
		out dx,al

		mov dx,VGA_CRTC_DATA
		in al,dx
	pop cx
	pop dx
	pop si
	ret
	
iowait:
    jmp .next2
    .next2 jmp .next3
    .next3 jmp .next4
    .next4 ret

align 16

global idt,idtinfo

section .isr
idtinfo dw endidt-idt-1
        dd idt
idt:
    times 0x6 dq 0
    .ill_op:
        .illopaddr dw 0
        dw 0x8 
        db 0
        db 0b10001110
        .illopaddrh dw 0
    times 0x6 dq 0
    .gpf:
        .gpfaddr dw 0
        dw 0x8
        db 0
        db 0b10001110
        .gpfaddrh dw 0
    .pf:
        .pfaddr dw 0
        dw 0x8 
        db 0
        db 0b10001110
        .pfaddrh dw 0
    times 0x11 dq 0
    .pit:
        .pitaddr dw 0 
        dw 0x8
        db 0
        db 0b10001110
        .pitaddrh dw 0 
    .key:
        .keyaddr dw 0
        dw 0x8
        db 0
        db 0b10001110
        .keyaddrh dw 0
    times 5 dq 0
    .irq7:
        .irq7addr dw 0
        dw 0x8
        db 0
        db 0b10001110
        .irq7addrh dw 0
    times 100 dq 0
endidt:

extern timer
extern state_dump
extern regs
jfadr: dd hang
       dw 0x8
       
extern geip
ds_val: dw 0
irq0:
    cli
    pop dword [geip]
    push dword [geip]
    pushad
    mov ax,ds
    mov [ds_val],ax
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov al,0x20
    out 0x20,al
    call timer    
    mov bx,[ds_val]
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    popad
    cmp dword [esp+4],0x8
    je s
    jmp $
    s:
    iretd
extern c_irq1
irq1:
    cli
    pushad
    mov ax,ds
    push ax
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov al,0x20
    out 0x20,al
    in al,60h
    call c_irq1    
    pop bx
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    popad
    iretd

global int0x80

int0x80:


    iret
;void flush_ps2_buffer();
flush_ps2_buffer:
    push ax
    in al,0x64
    test al,1
    in al,0x60
    jz flush_ps2_buffer
    pop ax
    ret

extern c_pfh
pagefaulthandler:
    call c_pfh
extern c_irq7 
irq7handler:
    
    iretd
    
dc:
    pushad
    mov eax,cr0
    or eax,60000000h    
    mov cr0,eax
    popad
    ret
ec:
    push eax
    mov eax,cr0
    and eax,0x9fffffff
    mov cr0,eax
    pop eax
    ret
global farjump
farjump:
    JMP far [esp+4]
    ret
global ec,dc

    global ok
section .data
    ok db "OK",10,0
    pciscancomplete db "PCI scan complete.",10,0
    teststr db "If you see this and your not a dev,restart your computer.",0
    versionstring db 10,"Starting NetDOS/32...",10,0
    pic2 db "Initializing PIC and interrupts...",0
    pci db "Scanning PCI...",10,0
    psoutofboundfailsafe db 0
    db 0
regs_90x60:
; MISC
	db 0Eh
; SEQuencer
	db 03h, 01h, 03h, 00h, 02h
; CRTC
	db  6Bh, 59h,  5Ah, 82h, 60h,  8Dh, 0Bh,  3Eh,
	db  00h, 47h,  06h, 07h, 00h,  00h, 00h,  00h,
	db 0EAh, 0Ch, 0DFh, 2Dh, 08h, 0E8h, 05h, 0A3h,
	db 0FFh
; GC (no)
; AC (no)
global loadpagedir
global enablepage
; 

loadpagedir:
    mov eax,[esp+4]
    mov cr3,eax
    ret
    
enablepage:
    push ebp
    mov ebp,esp
     mov eax,cr4
    or eax,0x10
    mov cr4,eax
    mov eax,cr0
    or eax,0x80000000
    mov cr0,eax
   
    mov esp,ebp
    pop ebp
    
    ret
global memtest
memtest:
    push edi
    push esi
    push ebx
    mov esi,0xaa55aa55
    mov edi,0x55aa55aa
    mov eax,[esp+12+4]
.l:
    mov ebx,eax
    add ebx,0xffc
    mov edx,[ebx]
    mov [ebx],esi
    xor dword [ebx],0xffffffff
    cmp edi,[ebx]
    jne .f
    xor dword [ebx],0xffffffff
    cmp esi,[ebx]
    jne .f
    mov [ebx],edx
    add eax,0x1000
    cmp eax,[esp+12+8]
    jbe .l
    pop ebx
    pop esi
    pop edi
    ret
.f:
    mov [ebx],edx
    pop ebx
    pop esi
    pop edi
    ret
bootinfo:
    rootdrvnum dd 0

    
