;; kernel
bits 32
section .text
	;multiboot spec
	align 4
	dd 0x1BADB002	; magic
	dd 0x00		; flags
	dd - (0x1BADB002 - 0x00) ; checksum. m+f+c should be zero


global start
global key_handlr
global read_port
global write_port
global load_idt

extern kmain		; kmain is defined in c file
extern cmain		; cmain is defined in c file
extern key_handlr_main  ; key_handlr_main is defined in c file

read_port:
	mov   edx, [esp + 4]
			     ; al is the lower 8 bits of eax
	in    al, dx         ; dx is the lower 16 bits of edx 
	ret

write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

load_idt:
	mov   edx, [esp + 4]
	lidt  [edx]
	sti
	ret

key_handlr:
	call  key_handlr_main
	iretd

start:
	cli		     ; blocks interrupts
	mov   esp, stack_space ; set stack pointer
	call  kmain

	mov   ecx, nl	     ; new line
	mov   edx, 0	     ; new line

	call  cmain
	cmp   edx, 1	     ; if edx is 0 jmp to exit (loop)
	je   _exit
	hlt		     ; halt the CPU

	
_exit:
	mov   ebx, 0
	mov   eax, 1	     ; sys_call for exit
	int   0x80

section .bss
resb 8192		; 8kb for stack
stack_space:

section .data
	nl db ' ', 0x0A
